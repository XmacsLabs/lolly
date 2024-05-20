
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

template <typename Stored, typename Regard_As= Stored, typename... Params>
static ref_counter_base<Regard_As>*
make (Params&&... p) {
  return reinterpret_cast<ref_counter_base<Regard_As>*> (
      tm_new<ref_counter<Stored>> (std::forward<Params> (p)...));
}

template <typename T> class counted_ptr_nullable {
protected:
  using base     = counted_ptr_nullable<T>;
  using counter_t= ref_counter_base<T>;
  counter_t* counter;
  explicit counted_ptr_nullable (counter_t* c) : counter (c) {}

public:
  counted_ptr_nullable () : counter (nullptr) {}
  counted_ptr_nullable (const counted_ptr_nullable<T>& x)
      : counter (x.counter) {
    if (counter != nullptr) {
      counter->inc_count ();
    }
  }
  ~counted_ptr_nullable () {
    if (counter != nullptr) {
      counter->dec_count ();
    }
  }
  counted_ptr_nullable<T>& operator= (counted_ptr_nullable<T>& x) {
    if (x.counter != nullptr) {
      x.counter->inc_count ();
    }
    if (this->counter != nullptr) {
      this->counter->dec_count ();
    }
    this->counter= x.counter;
    return *this;
  }
  T* get () {
    if (counter != nullptr) {
      return counter->get ();
    }
    else {
      return nullptr;
    }
  }
  const T* get () const { return get (); }
  T*       operator->() { return get (); }
  const T* operator->() const { return get (); }
  bool     is_nil () const { return counter == nullptr; }
};

template <typename T>
inline bool
is_nil (const counted_ptr_nullable<T> x) {
  return x.is_nil ();
}
