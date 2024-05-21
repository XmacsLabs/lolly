
#pragma once

#include "fast_alloc.hpp"
#include <utility>

template <typename T> struct ref_counter_base {

  virtual void inc_count ()= 0;
  virtual void dec_count ()= 0;
  virtual T*   get ()      = 0;
};

template <typename T> struct ref_counter : ref_counter_base<T> {
  /// @brief the reference count of the object
  int ref_count;
  T   content;
  template <typename... Params>
  explicit ref_counter (Params&&... p)
      : ref_count (1), content (std::forward<Params> (p)...){};

  void inc_count () { ref_count++; }
  void dec_count () {
    ref_count--;
    if (ref_count == 0) {
      tm_delete (this);
    }
  }
  T* get () { return &content; }
};

template <typename Stored, typename Regard_As= Stored, typename... Params>
static ref_counter_base<Regard_As>*
make_derived (Params&&... p) {
  return reinterpret_cast<ref_counter_base<Regard_As>*> (
      tm_new<ref_counter<Stored>> (std::forward<Params> (p)...));
}

template <typename T, bool nullable= false> class counted_ptr {
protected:
  using base     = counted_ptr<T, nullable>;
  using counter_t= ref_counter_base<T>;
  counter_t* counter;
  explicit counted_ptr (counter_t* c) : counter (c) {}
  template <typename... Params> static counter_t* make (Params&&... p) {
    return make_derived<T, T> (std::forward<Params> (p)...);
  }

public:
  counted_ptr () : counter (nullptr) {}
  counted_ptr (const counted_ptr<T, nullable>& x) : counter (x.counter) {
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
    return *this;
  }
  T* get () {
    if constexpr (nullable) {
      if (counter != nullptr) {
        return counter->get ();
      }
      else {
        return nullptr;
      }
    }
    else {
      return counter->get ();
    }
  }
  const T* get () const { return get (); }
  T*       operator->() { return get (); }
  const T* operator->() const { return get (); }
  bool     is_nil () const {
    if constexpr (nullable) {
      return counter == nullptr;
    }
    else {
      return false;
    }
  }
};

template <typename T>
inline bool
is_nil (const counted_ptr<T, true> x) {
  return x.is_nil ();
}
