
#pragma once

#include "fast_alloc.hpp"
#include <utility>

template <typename T> struct ref_counter {
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
};

template <typename T> class counted_ptr {
protected:
  using base     = counted_ptr<T>;
  using counter_t= ref_counter<T>;
  counter_t* counter;
  T*         get () { return &(counter->content); }
  const T*   get () const { return &(counter->content); }
  template <typename... Params> static counter_t* make (Params&&... p) {
    return tm_new<counter_t> (std::forward<Params> (p)...);
  }
  explicit counted_ptr (counter_t* c) : counter (c) {}

public:
  counted_ptr (const counted_ptr<T>& x) : counter (x.counter) {
    counter->inc_count ();
  }
  ~counted_ptr () { counter->dec_count (); }
  counted_ptr<T>& operator= (counted_ptr& x) {
    x.counter->inc_count ();
    this->counter->dec_count ();
    this->counter= x.counter;
    return *this;
  }
  T*       operator->() { return &(counter->content); }
  const T* operator->() const { return &(counter->content); }
};
