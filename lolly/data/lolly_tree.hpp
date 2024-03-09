
/** \file lolly_lolly_tree.hpp
 *  \copyright GPLv3
 *  \details lolly_tree with data
 *  \author Darcy Shen
 *  \date   2024
 */

#pragma once

#include "array.hpp"
#include "basic.hpp"
#include "list.hpp"
#include "string.hpp"

namespace lolly {
namespace data {

template <typename T> class lolly_tree;
template <typename T> class lolly_tree_rep;
template <typename T> class atomic_rep;
template <typename T> class compound_rep;
class blackbox;

template <typename T> class lolly_tree {
  lolly_tree_rep<T>* rep; // can be atomic or compound or generic

public:
  inline lolly_tree (lolly_tree_rep<T>* rep2) : rep (rep2) { rep->ref_count++; }

  inline lolly_tree (const lolly_tree<T>& x) : rep (x.rep) { rep->ref_count++; }

  inline atomic_rep<T>* operator->() {
    // CHECK_ATOMIC (*this);
    return static_cast<atomic_rep<T>*> (rep);
  }

  inline lolly_tree () : rep (tm_new<atomic_rep<T>> (string ())) {}
  inline lolly_tree (string l) : rep (tm_new<atomic_rep<T>> (l)) {}
  inline lolly_tree (const char* l) : rep (tm_new<atomic_rep<T>> (l)) {}

  inline lolly_tree (int l, int n= 0)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (n))) {}

  inline lolly_tree (int l, array<lolly_tree<T>> a)
      : rep (tm_new<compound_rep<T>> (l, a)) {}

  inline lolly_tree (lolly_tree<T> t, int n)
      : rep (tm_new<compound_rep<T>> (t.rep->op, array<lolly_tree<T>> (n))) {
    // CHECK_COMPOUND (t);
  }

  inline lolly_tree (int l, lolly_tree<T> t1)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (1))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
  }

  inline lolly_tree (int l, lolly_tree<T> t1, lolly_tree<T> t2)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (2))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
    (static_cast<compound_rep<T>*> (rep))->a[1]= t2;
  }

  inline lolly_tree (int l, lolly_tree<T> t1, lolly_tree<T> t2,
                     lolly_tree<T> t3)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (3))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
    (static_cast<compound_rep<T>*> (rep))->a[1]= t2;
    (static_cast<compound_rep<T>*> (rep))->a[2]= t3;
  }

  inline lolly_tree (int l, lolly_tree<T> t1, lolly_tree<T> t2,
                     lolly_tree<T> t3, lolly_tree<T> t4)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (4))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
    (static_cast<compound_rep<T>*> (rep))->a[1]= t2;
    (static_cast<compound_rep<T>*> (rep))->a[2]= t3;
    (static_cast<compound_rep<T>*> (rep))->a[3]= t4;
  }

  inline lolly_tree (int l, lolly_tree<T> t1, lolly_tree<T> t2,
                     lolly_tree<T> t3, lolly_tree<T> t4, lolly_tree<T> t5)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (5))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
    (static_cast<compound_rep<T>*> (rep))->a[1]= t2;
    (static_cast<compound_rep<T>*> (rep))->a[2]= t3;
    (static_cast<compound_rep<T>*> (rep))->a[3]= t4;
    (static_cast<compound_rep<T>*> (rep))->a[4]= t5;
  }

  inline lolly_tree (int l, lolly_tree<T> t1, lolly_tree<T> t2,
                     lolly_tree<T> t3, lolly_tree<T> t4, lolly_tree<T> t5,
                     lolly_tree<T> t6)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (6))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
    (static_cast<compound_rep<T>*> (rep))->a[1]= t2;
    (static_cast<compound_rep<T>*> (rep))->a[2]= t3;
    (static_cast<compound_rep<T>*> (rep))->a[3]= t4;
    (static_cast<compound_rep<T>*> (rep))->a[4]= t5;
    (static_cast<compound_rep<T>*> (rep))->a[5]= t6;
  }

  inline lolly_tree (int l, lolly_tree<T> t1, lolly_tree<T> t2,
                     lolly_tree<T> t3, lolly_tree<T> t4, lolly_tree<T> t5,
                     lolly_tree<T> t6, lolly_tree<T> t7)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (7))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
    (static_cast<compound_rep<T>*> (rep))->a[1]= t2;
    (static_cast<compound_rep<T>*> (rep))->a[2]= t3;
    (static_cast<compound_rep<T>*> (rep))->a[3]= t4;
    (static_cast<compound_rep<T>*> (rep))->a[4]= t5;
    (static_cast<compound_rep<T>*> (rep))->a[5]= t6;
    (static_cast<compound_rep<T>*> (rep))->a[6]= t7;
  }

  inline lolly_tree (int l, lolly_tree<T> t1, lolly_tree<T> t2,
                     lolly_tree<T> t3, lolly_tree<T> t4, lolly_tree<T> t5,
                     lolly_tree<T> t6, lolly_tree<T> t7, lolly_tree<T> t8)
      : rep (tm_new<compound_rep<T>> (l, array<lolly_tree<T>> (8))) {
    (static_cast<compound_rep<T>*> (rep))->a[0]= t1;
    (static_cast<compound_rep<T>*> (rep))->a[1]= t2;
    (static_cast<compound_rep<T>*> (rep))->a[2]= t3;
    (static_cast<compound_rep<T>*> (rep))->a[3]= t4;
    (static_cast<compound_rep<T>*> (rep))->a[4]= t5;
    (static_cast<compound_rep<T>*> (rep))->a[5]= t6;
    (static_cast<compound_rep<T>*> (rep))->a[6]= t7;
    (static_cast<compound_rep<T>*> (rep))->a[7]= t8;
  }

  lolly_tree<T>& operator= (lolly_tree<T> x) {
    x.rep->ref_count++;
    if ((--rep->ref_count) == 0) destroy_tree_rep (rep);
    rep= x.rep;
    return *this;
  }

  ~lolly_tree () {
    if ((--rep->ref_count) == 0) {
      destroy_tree_rep (rep);
      rep= NULL;
    }
  }

  inline lolly_tree<T>& operator[] (int i) {
    // CHECK_COMPOUND (*this);
    return (static_cast<compound_rep<T>*> (rep))->a[i];
  }

  inline lolly_tree<T> operator() (int start, int end) {
    lolly_tree<T> r (rep->op, end - start);
    for (int i= start; i < end; i++)
      r[i - start]= (static_cast<compound_rep<T>*> (rep))->a[i];
    return r;
  }

  template <typename T> friend inline int N (lolly_tree<T> t);
  template <typename T> friend inline int arity (lolly_tree<T> t);
  template <typename T> friend inline array<lolly_tree<T>> A (lolly_tree<T> t);
  template <typename T>
  friend inline array<lolly_tree<T>>&      AR (lolly_tree<T> t);
  template <typename T> friend inline bool is_atomic (lolly_tree<T> t);
  template <typename T> friend inline bool is_compound (lolly_tree<T> t);
  template <typename T> friend inline bool is_generic (lolly_tree<T> t);
  template <typename T>
  friend inline bool operator== (lolly_tree<T> t, int lab);
  template <typename T>
  friend inline bool operator!= (lolly_tree<T> t, int lab);
  template <typename T>
  friend inline bool operator== (lolly_tree<T> t, string s);
  template <typename T>
  friend inline bool operator!= (lolly_tree<T> t, string s);
  template <typename T>
  friend inline bool operator== (lolly_tree<T> t, const char* s);
  template <typename T>
  friend inline bool operator!= (lolly_tree<T> t, const char* s);
  template <typename T>
  friend inline lolly_tree_rep<T>* inside (lolly_tree<T> t);
  template <typename T>
  friend inline bool strong_equal (lolly_tree<T> t, lolly_tree<T> u);
  template <typename T> friend inline bool is_func (lolly_tree<T> t, int l);
  template <typename T>
  friend inline bool is_func (lolly_tree<T> t, int l, int i);

  template <typename T> friend lolly_tree<T> copy (lolly_tree<T> t);
  template <typename T>
  friend bool operator== (lolly_tree<T> t, lolly_tree<T> u);
  template <typename T>
  friend bool operator!= (lolly_tree<T> t, lolly_tree<T> u);
  template <typename T>
  friend lolly_tree<T> operator* (lolly_tree<T> t1, lolly_tree<T> t2);
  template <typename T>
  friend lolly_tree<T>& operator<< (lolly_tree<T>& t, lolly_tree<T> t2);
  template <typename T>
  friend lolly_tree<T>& operator<< (lolly_tree<T>& t, array<lolly_tree<T>> a);
  template <typename T>
  friend tm_ostream& operator<< (tm_ostream& out, lolly_tree<T> t);
  template <typename T> friend list<lolly_tree<T>> as_trees (list<pointer> l);
  template <typename T> friend blackbox as_blackbox (const lolly_tree<T>& t);
};

template <typename T> class lolly_tree_rep : concrete_struct {
public:
  int op;
  T   data;
  inline lolly_tree_rep (int op2) : op (op2) {}
  friend class lolly_tree<T>;
};

template <typename T> class atomic_rep : public lolly_tree_rep<T> {
public:
  string label;
  // the tree_label of op=0 in TeXmacs is STRING
  inline atomic_rep (string l) : lolly_tree_rep<T> (0), label (l) {}
  friend class lolly_tree<T>;
};

template <typename T> class compound_rep : public lolly_tree_rep<T> {
public:
  array<lolly_tree<T>> a;
  inline compound_rep (int l, array<lolly_tree<T>> a2)
      : lolly_tree_rep<T> (l), a (a2) {}
  friend class lolly_tree<T>;
};

template <typename T>
static void
destroy_tree_rep (lolly_tree_rep<T>* rep) {
  if (((int) rep->op) == 0) tm_delete (static_cast<atomic_rep<T>*> (rep));
  else if (((int) rep->op) > 0) tm_delete (static_cast<compound_rep<T>*> (rep));
}

template <typename T>
inline int
N (lolly_tree<T> t) {
  // CHECK_COMPOUND (t);
  return N ((static_cast<compound_rep<T>*> (t.rep))->a);
}

template <typename T>
inline int
arity (lolly_tree<T> t) {
  if (t.rep->op == /*STRING*/ 0) return 0;
  else return N ((static_cast<compound_rep<T>*> (t.rep))->a);
}

template <typename T>
inline array<lolly_tree<T>>
A (lolly_tree<T> t) {
  // CHECK_COMPOUND (t);
  return (static_cast<compound_rep<T>*> (t.rep))->a;
}

template <typename T>
inline array<lolly_tree<T>>&
AR (lolly_tree<T> t) {
  // CHECK_COMPOUND (t);
  return (static_cast<compound_rep<T>*> (t.rep))->a;
}

template <typename T>
inline bool
is_atomic (lolly_tree<T> t) {
  return (((int) t.rep->op) == 0);
}

template <typename T>
inline bool
is_compound (lolly_tree<T> t) {
  return (((int) t.rep->op) > /*STRING*/ 0);
}

template <typename T>
inline bool
is_generic (lolly_tree<T> t) {
  return ((int) t.rep->op) < 0;
}

template <typename T>
inline bool
operator== (lolly_tree<T> t, int lab) {
  return (t.rep->op == lab) && (N (t) == 0);
}

template <typename T>
inline bool
operator!= (lolly_tree<T> t, int lab) {
  return (t.rep->op != lab) || (N (t) != 0);
}

template <typename T>
inline bool
operator== (lolly_tree<T> t, string s) {
  return (t.rep->op == /*STRING*/ 0) && (t->label == s);
}

template <typename T>
inline bool
operator!= (lolly_tree<T> t, string s) {
  return (t.rep->op != /*STRING*/ 0) || (t->label != s);
}

template <typename T>
inline bool
operator== (lolly_tree<T> t, const char* s) {
  return (t.rep->op == /*STRING*/ 0) && (t->label == s);
}

template <typename T>
inline bool
operator!= (lolly_tree<T> t, const char* s) {
  return (t.rep->op != /*STRING*/ 0) || (t->label != s);
}

template <typename T>
inline bool
operator== (lolly_tree<T> t, lolly_tree<T> u) {
  if (strong_equal (t, u)) return true;
  return (t->op == u->op) &&
         (is_atomic (t) ? (t->label == u->label) : (A (t) == A (u)));
}

template <typename T>
inline bool
operator!= (lolly_tree<T> t, lolly_tree<T> u) {
  if (strong_equal (t, u)) return false;
  return (t->op != u->op) ||
         (is_atomic (t) ? (t->label != u->label) : (A (t) != A (u)));
}

template <typename T>
inline lolly_tree_rep<T>*
inside (lolly_tree<T> t) {
  return t.rep;
}

template <typename T>
inline bool
strong_equal (lolly_tree<T> t, lolly_tree<T> u) {
  return t.rep == u.rep;
}

template <typename T>
inline bool
is_func (lolly_tree<T> t, int l) {
  return (t.rep->op == l) && (N (t) != 0);
}

template <typename T>
inline bool
is_func (lolly_tree<T> t, int l, int i) {
  return (t.rep->op == l) && (N (t) == i);
}

template <typename T>
inline bool
is_bool (lolly_tree<T> t) {
  return is_atomic (t) && is_bool (t->label);
}

template <typename T>
inline bool
is_int (lolly_tree<T> t) {
  return is_atomic (t) && is_int (t->label);
}

template <typename T>
inline bool
is_double (lolly_tree<T> t) {
  return is_atomic (t) && is_double (t->label);
}

template <typename T>
inline bool
is_string (lolly_tree<T> t) {
  return is_atomic (t);
}

template <typename T>
inline bool
as_bool (lolly_tree<T> t) {
  if (is_atomic (t)) return as_bool (t->label);
  else return false;
}

template <typename T>
inline int
as_int (lolly_tree<T> t) {
  if (is_atomic (t)) return as_int (t->label);
  else return 0;
}

template <typename T>
inline long int
as_long_int (lolly_tree<T> t) {
  if (is_atomic (t)) return as_long_int (t->label);
  else return 0;
}

template <typename T>
inline double
as_double (lolly_tree<T> t) {
  if (is_atomic (t)) return as_double (t->label);
  else return 0.0;
}

template <typename T>
inline string
as_string (lolly_tree<T> t) {
  if (is_atomic (t)) return t->label;
  else return "";
}

template <typename T> lolly_tree<T> copy (lolly_tree<T> t);
template <typename T>
lolly_tree<T> operator* (lolly_tree<T> t1, lolly_tree<T> t2);
template <typename T>
lolly_tree<T>& operator<< (lolly_tree<T>& t, lolly_tree<T> t2);
template <typename T>
lolly_tree<T>& operator<< (lolly_tree<T>& t, array<lolly_tree<T>> a);
template <typename T> tm_ostream& operator<< (tm_ostream& out, lolly_tree<T> t);

} // namespace data
} // namespace lolly