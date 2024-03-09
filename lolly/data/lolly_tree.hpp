
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
  inline lolly_tree (lolly_tree_rep<T>* rep2) : rep (rep2){rep->ref_count++};
  inline lolly_tree (const lolly_tree& x) : rep (x.rep){rep->ref_count++};
  inline ~lolly_tree () {
    if ((--rep->ref_count) == 0) {
      // destroy_tree_rep (rep);
      rep= NULL;
    }
  };
  inline atomic_rep<T>* operator->();
  inline lolly_tree&    operator= (lolly_tree x);

  inline lolly_tree ();
  inline lolly_tree (string l);
  inline lolly_tree (const char* l);
  inline lolly_tree (int l, int n= 0);
  inline lolly_tree (int l, array<lolly_tree> a);
  inline lolly_tree (lolly_tree t, int n);
  lolly_tree (int l, lolly_tree t1);
  lolly_tree (int l, lolly_tree t1, lolly_tree t2);
  lolly_tree (int l, lolly_tree t1, lolly_tree t2, lolly_tree t3);
  lolly_tree (int l, lolly_tree t1, lolly_tree t2, lolly_tree t3,
              lolly_tree t4);
  lolly_tree (int l, lolly_tree t1, lolly_tree t2, lolly_tree t3, lolly_tree t4,
              lolly_tree t5);
  lolly_tree (int l, lolly_tree t1, lolly_tree t2, lolly_tree t3, lolly_tree t4,
              lolly_tree t5, lolly_tree t6);
  lolly_tree (int l, lolly_tree t1, lolly_tree t2, lolly_tree t3, lolly_tree t4,
              lolly_tree t5, lolly_tree t6, lolly_tree t7);
  lolly_tree (int l, lolly_tree t1, lolly_tree t2, lolly_tree t3, lolly_tree t4,
              lolly_tree t5, lolly_tree t6, lolly_tree t7, lolly_tree t8);
  inline lolly_tree<T>& operator[] (int i);
  lolly_tree<T>         operator() (int start, int end);

  friend inline int                   N (lolly_tree<T> t);
  friend inline int                   arity (lolly_tree<T> t);
  friend inline array<lolly_tree<T>>  A (lolly_tree<T> t);
  friend inline array<lolly_tree<T>>& AR (lolly_tree<T> t);
  friend inline bool                  is_atomic (lolly_tree<T> t);
  friend inline bool                  is_compound (lolly_tree<T> t);
  friend inline bool                  is_generic (lolly_tree<T> t);
  friend inline bool                  operator== (lolly_tree<T> t, int lab);
  friend inline bool                  operator!= (lolly_tree<T> t, int lab);
  friend inline bool                  operator== (lolly_tree<T> t, string s);
  friend inline bool                  operator!= (lolly_tree<T> t, string s);
  friend inline bool               operator== (lolly_tree<T> t, const char* s);
  friend inline bool               operator!= (lolly_tree<T> t, const char* s);
  friend inline lolly_tree_rep<T>* inside (lolly_tree<T> t);
  friend inline bool strong_equal (lolly_tree<T> t, lolly_tree<T> u);
  friend inline bool is_func (lolly_tree<T> t, int l);
  friend inline bool is_func (lolly_tree<T> t, int l, int i);

  template <typename T> friend lolly_tree<T> copy (lolly_tree<T> t);
  friend bool        operator== (lolly_tree<T> t, lolly_tree<T> u);
  friend bool        operator!= (lolly_tree<T> t, lolly_tree<T> u);
  friend lolly_tree& operator<< (lolly_tree<T>& t, lolly_tree<T> t2);
  friend lolly_tree& operator<< (lolly_tree<T>& t, array<lolly_tree<T>> a);
  friend tm_ostream& operator<< (tm_ostream& out, lolly_tree<T> t);
  friend lolly_tree  operator* (lolly_tree<T> t1, lolly_tree<T> t2);
  friend list<lolly_tree<T>> as_trees (list<pointer> l);
  friend blackbox            as_blackbox (const lolly_tree<T>& t);
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

template <typename T> lolly_tree<T> copy (lolly_tree<T> t);

} // namespace data
} // namespace lolly