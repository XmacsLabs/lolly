/** \file list.hpp
 *  \copyright GPLv3
 *  \details linked lists with reference counting
 *  \author Joris van der Hoeven
 *  \date   1999
 */

#ifndef LIST_H
#define LIST_H

#include "basic.hpp"

template <class T> class list_rep;
template <class T> class list;

template <class T> bool is_nil (list<T> l);
template <class T> bool is_atom (list<T> l);
template <class T> bool strong_equal (list<T> l1, list<T> l2);

template <class T> class list {
  CONCRETE_NULL_TEMPLATE (list, T);
  inline list (T item);
  inline list (T item, list<T> next);
  inline list (T item1, T item2, list<T> next);
  inline list (T item1, T item2, T item3, list<T> next);
  T&             operator[] (int i);
  static list<T> init;

  friend bool is_atom      LESSGTR (list<T> l);
  friend bool strong_equal LESSGTR (list<T> l1, list<T> l2);
};

extern int list_count;
template <class T> class list_rep : concrete_struct {
public:
  T       item;
  list<T> next;

  inline list_rep<T> (T item2, list<T> next2) : item (item2), next (next2) {
    TM_DEBUG (list_count++);
  }
  inline ~list_rep<T> () { TM_DEBUG (list_count--); }
  friend class list<T>;
};

CONCRETE_NULL_TEMPLATE_CODE (list, class, T);
#define TMPL template <class T>
TMPL inline list<T>::list (T item)
    : rep (tm_new<list_rep<T>> (item, list<T> ())) {}
TMPL inline list<T>::list (T item, list<T> next)
    : rep (tm_new<list_rep<T>> (item, next)) {}
TMPL inline list<T>::list (T item1, T item2, list<T> next)
    : rep (tm_new<list_rep<T>> (item1, list<T> (item2, next))) {}
TMPL inline list<T>::list (T item1, T item2, T item3, list<T> next)
    : rep (tm_new<list_rep<T>> (item1, list<T> (item2, item3, next))) {}
TMPL inline bool
is_atom (list<T> l) {
  return (!is_nil (l)) && is_nil (l->next);
}
TMPL list<T> list<T>::init= list<T> ();

TMPL int N (list<T> l);
TMPL list<T> copy (list<T> l);
TMPL list<T> operator* (list<T> l1, T x);
TMPL list<T> operator* (list<T> l1, list<T> l2);
TMPL list<T> head (list<T> l, int n= 1);
TMPL list<T> tail (list<T> l, int n= 1);

/**
 * @brief Return the last item of the list.
 * The input list must not be an empty list.
 *
 * @tparam T
 * @param l the list
 * @return T
 */
TMPL T  last_item (list<T> l);
TMPL T& access_last (list<T>& l);
TMPL list<T>& suppress_last (list<T>& l);
TMPL list<T> reverse (list<T> l);
TMPL list<T> remove (list<T> l, T what);
TMPL bool    contains (list<T> l, T what);

TMPL tm_ostream& operator<< (tm_ostream& out, list<T> l);
TMPL list<T>& operator<< (list<T>& l, T item);
TMPL list<T>& operator<< (list<T>& l1, list<T> l2);
TMPL list<T>& operator>> (T item, list<T>& l);
TMPL list<T>& operator<< (T& item, list<T>& l);
TMPL bool     operator== (list<T> l1, list<T> l2);
TMPL bool     operator!= (list<T> l1, list<T> l2);
TMPL bool     operator< (list<T> l1, list<T> l2);
TMPL bool     operator<= (list<T> l1, list<T> l2);
#undef TMPL

#include "list.ipp"

#endif // defined LIST_H
