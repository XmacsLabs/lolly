#include "lolly_tree.hpp"

namespace lolly {
namespace data {

template <typename T>
static void
destroy_tree_rep (lolly_tree_rep<T>* rep) {
  if (((int) rep->op) == 0) tm_delete (static_cast<atomic_rep<T>*> (rep));
  else if (((int) rep->op) > 0) tm_delete (static_cast<compound_rep<T>*> (rep));
}

template <typename T>
lolly_tree<T>&
lolly_tree<T>::operator= (lolly_tree<T> x) {
  x.rep->ref_count++;
  if ((--rep->ref_count) == 0) destroy_tree_rep (rep);
  rep= x.rep;
  return *this;
}

template <typename T> lolly_tree<T>::~lolly_tree () {
  if ((--rep->ref_count) == 0) {
    destroy_tree_rep (rep);
    rep= NULL;
  }
}

template <typename T>
lolly_tree<T>
copy (lolly_tree<T> t) {
  if (is_atomic (t)) return tree (copy (t->label));
  else {
    int           i, n= N (t);
    lolly_tree<T> t2 (t, n);
    for (i= 0; i < n; i++)
      t2[i]= copy (t[i]);
    return t2;
  }
}

template <typename T>
bool
operator== (lolly_tree<T> t, lolly_tree<T> u) {
  if (strong_equal (t, u)) return true;
  return (t->op == u->op) &&
         (is_atomic (t) ? (t->label == u->label) : (A (t) == A (u)));
}

template <typename T>
bool
operator!= (lolly_tree<T> t, lolly_tree<T> u) {
  if (strong_equal (t, u)) return false;
  return (t->op != u->op) ||
         (is_atomic (t) ? (t->label != u->label) : (A (t) != A (u)));
}

template <typename T>
lolly_tree<T>
operator* (lolly_tree<T> t1, lolly_tree<T> t2) {
  int i;
  if (is_atomic (t1)) t1= tree (t2->op, t1);
  if (is_atomic (t2)) t2= tree (t1->op, t2);
  lolly_tree<T> r (t1, N (t1) + N (t2));
  for (i= 0; i < N (t1); i++)
    r[i]= t1[i];
  for (i= 0; i < N (t2); i++)
    r[i + N (t1)]= t2[i];
  return r;
}

template <typename T>
lolly_tree<T>&
operator<< (lolly_tree<T>& t, lolly_tree<T> t2) {
  // CHECK_COMPOUND (t);
  (static_cast<compound_rep<T>*> (t.rep))->a << t2;
  return t;
}

template <typename T>
lolly_tree<T>&
operator<< (lolly_tree<T>& t, array<lolly_tree<T>> a) {
  // CHECK_COMPOUND (t);
  (static_cast<compound_rep<T>*> (t.rep))->a << a;
  return t;
}

template <typename T>
tm_ostream&
operator<< (tm_ostream& out, lolly_tree<T> t) {
  if (is_atomic (t)) return out << t->label;
  else if (is_compound (t)) {
    int i, n= N (t);
    out << as_string (t->op);
    if (n == 0) return out << "()";
    out << " (";
    for (i= 0; i < n - 1; i++)
      out << t[i] << ", ";
    out << t[i] << ")";
    return out;
  }
  else out << as_blackbox (t);
  return out;
}

} // namespace data
} // namespace lolly
