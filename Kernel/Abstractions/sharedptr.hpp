/** \file sharedptr.hpp
 *  \copyright GPLv3
 *  \details Defines smart pointers with reference counting. No spin lock is
 * applied.
 *  \author jingkaimori
 *  \date   2024
 */

#pragma once

#include "fast_alloc.hpp"
#include <utility>

struct ref_counter_base {
  virtual void  inc_count ()= 0;
  virtual void  dec_count ()= 0;
  virtual void* get ()      = 0;
};

/**
 * @brief Structure representing an object with a reference count.
 * @tparam T Actual type of inner object, must be a complete type
 */
template <typename T> struct ref_counter : ref_counter_base {
  /// @brief the reference count of the object
  int ref_count;
  T   content;

  /**
   * @brief in-place construct an object, and set its reference count to 1.
   * @tparam Params types of parameters required by constructor of object
   */
  template <typename... Params>
  explicit ref_counter (Params&&... p)
      : ref_count (1), content (std::forward<Params> (p)...){};

  void inc_count () { ref_count++; }
  /**
   * @brief decrement the reference count of the object and delete it if the
   * count reaches 0.
   */
  void dec_count () {
    ref_count--;
    if (ref_count == 0) {
      tm_delete (this);
    }
  }
  void* get () { return &content; }
};

/**
 * @brief Smart pointer with reference counting.
 * @details
 * To use smart pointer, counted_ptr class should be inherited publicly, and the
 * type exploited to user of the smart pointer should be provided. If null
 * pointer may be held inside the smart pointer, parameter nullable should be
 * set to true explictly.
 *
 * The class derived from pointer should provide proper constructor so that user
 * can instantiate pointer from arguments of other type. Operator should be
 * overloaded at the derived class, rather than the type of underlying object.
 * @tparam T The type which user of pointer get from indirection of the smart
 * pointer.
 */
template <typename T, bool nullable= false> class counted_ptr {

protected:
  using counter_t= ref_counter_base;
  /**
   * @brief short-hand name of the class, avoiding duplicated type name of
   * underlying object inside template. If the derived class is also template
   * class, this declaration should be imported explictly with `using` clause.
   */
  using base= counted_ptr<T, nullable>;
  explicit counted_ptr (counter_t* c)
      : counter (c), rep (static_cast<T*> (c->get ())) {}
  /**
   * @tparam Stored the real type of underlying object, may differ from types
   * exploits to user. This type must be complete type.
   */
  template <typename Stored= T, typename... Params>
  static counter_t* make (Params&&... p) {
    return tm_new<ref_counter<Stored>> (std::forward<Params> (p)...);
  }

private:
  /**
   * @brief Opaque pointer to the counter that holds the instance of the
   * underlying object
   */
  counter_t* counter;

public:
  /**
   * cached pointer to the underlying object
   */
  T* rep;

  counted_ptr () : counter (nullptr), rep (nullptr) {
    static_assert (nullable,
                   "null pointer is not allowed in non-null smart pointer.");
  }
  /**
   * copy constructor of the smart pointer
   */
  counted_ptr (const counted_ptr<T, nullable>& x)
      : counter (x.counter), rep (x.rep) {
    if constexpr (nullable) {
      if (counter != nullptr) {
        counter->inc_count ();
      }
    }
    else {
      counter->inc_count ();
    }
  }
  ~counted_ptr () {
    if constexpr (nullable) {
      if (counter != nullptr) {
        counter->dec_count ();
      }
    }
    else {
      counter->dec_count ();
    }
  }
  /**
   * @brief Decrement the reference count for the old object (`*this`) and
   * increments the reference count for the new object.
   * @param x the new object
   */
  counted_ptr<T, nullable>& operator= (counted_ptr<T, nullable>& x) {
    if constexpr (nullable) {
      if (x.counter != nullptr) {
        x.counter->inc_count ();
      }
      if (this->counter != nullptr) {
        this->counter->dec_count ();
      }
    }
    else {
      x.counter->inc_count ();
      this->counter->dec_count ();
    }
    this->counter= x.counter;
    this->rep    = x.rep;
    return *this;
  }
  /**
   * @brief Steal underlying reference counter from rvalue temprary object, then
   * leave the old counter (`this->counter`) in the temprary object. Finally the
   * temprary object will be destroyed along with old object held by `*this`
   * @param x the new object
   */
  counted_ptr<T, nullable>& operator= (counted_ptr<T, nullable>&& x) {
    std::swap (this->counter, x.counter);
    std::swap (this->rep, x.rep);
    return *this;
  }
  /**
   * @brief pointer dereference operator
   */
  T* operator->() {
    if constexpr (nullable) {
      if (counter != nullptr) {
        return rep;
      }
      else {
        return nullptr;
      }
    }
    else {
      return rep;
    }
  }
  /**
   * @brief pointer dereference operator, call from const pointer to get const
   * object
   */
  const T* operator->() const { return operator->(); }
  bool     is_nil () const {
    if constexpr (nullable) {
      return counter == nullptr;
    }
    else {
      return false;
    }
  }
};

/**
 * @brief adapter of legacy is_nil function.
 */
template <typename T>
inline bool
is_nil (const counted_ptr<T, true> x) {
  return x.is_nil ();
}
