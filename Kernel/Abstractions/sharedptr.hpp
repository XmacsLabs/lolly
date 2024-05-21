
#pragma once

#include "fast_alloc.hpp"
#include <utility>

struct ref_counter_base {
  virtual void  inc_count ()= 0;
  virtual void  dec_count ()= 0;
  virtual void* get ()      = 0;
};

template <typename T> struct ref_counter : ref_counter_base {
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
  void* get () { return &content; }
};
template <typename T, bool nullable= false> class counted_ptr {

protected:
  using counter_t= ref_counter_base;
  using base     = counted_ptr<T, nullable>;
  explicit counted_ptr (counter_t* c)
      : counter (c), rep (static_cast<T*> (c->get ())) {}
  template <typename Stored= T, typename... Params>
  static counter_t* make (Params&&... p) {
    return tm_new<ref_counter<Stored>> (std::forward<Params> (p)...);
  }

private:
  counter_t* counter;

public:
  T* rep;
  counted_ptr () : counter (nullptr), rep (nullptr) {}
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
  counted_ptr<T, nullable>& operator= (counted_ptr<T, nullable>&& x) {
    std::swap (this->counter, x.counter);
    std::swap (this->rep, x.rep);
    return *this;
  }
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

template <typename T>
inline bool
is_nil (const counted_ptr<T, true> x) {
  return x.is_nil ();
}
