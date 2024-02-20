
/******************************************************************************
 * MODULE     : string.hpp
 * DESCRIPTION: Fixed size strings with reference counting and
 *              pointer copying. Zero-characters are allowed in strings.
 * COPYRIGHT  : (C) 1999  Joris van der Hoeven
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "basic.hpp"

namespace lolly {
namespace data {
using char8_t= int8_t;

template <typename T> class tm_string_view;
template <typename T> class tm_string;
template <typename T> int N (tm_string<T> a);
template <typename T> class tm_string_rep : concrete_struct {
  int n;
  T*  a;

public:
  inline tm_string_rep () : n (0), a (NULL) {}
  tm_string_rep (int n);
  inline ~tm_string_rep () {
    if (n != 0) tm_delete_array (a);
  }
  void resize (int n);

  friend class tm_string<T>;
  friend int N<> (tm_string<T> a);
};

template <typename T> class tm_string {
  CONCRETE_TEMPLATE (tm_string, T);
  inline tm_string () : rep (tm_new<tm_string_rep<T>> ()) {}
  inline explicit tm_string (int n) : rep (tm_new<tm_string_rep<T>> (n)) {}
  tm_string (T c);
  tm_string (const tm_string_view<T>& c);
  tm_string (T c, int n);
  template <size_t N> tm_string (const T (&s)[N]);
  inline T* buffer () { return rep->a; }
  inline T* buffer (int size) {
    rep->resize (size);
    return rep->a;
  }
  inline operator tm_string_view<T> () {
    return tm_string_view<T> (rep->n, rep->a);
  }
  inline T&         operator[] (int i) { return rep->a[i]; }
  tm_string_view<T> operator() (int start, int end);
};
CONCRETE_TEMPLATE_CODE (tm_string, typename, T);

using string   = tm_string<char>;
using stringU16= tm_string<char16_t>;
using stringU8 = tm_string<char16_t>;

template <typename T> class tm_string_view {
public:
  const int      N;
  const T* const a;
  tm_string_view (int N_, const T* a_) : N (N_), a (a_){};
  template <size_t N_>
  constexpr tm_string_view (const T null_end_str[N_])
      : N (N_), a (null_end_str){};
  tm_string_view ()                                     = delete;
  tm_string_view (const tm_string_view<T>&)             = delete;
  tm_string_view (tm_string_view<T>&&)                  = default;
  tm_string_view&   operator= (const tm_string_view<T>&)= delete;
  tm_string_view&   operator= (tm_string_view<T>&&)     = delete;
  tm_string_view<T> operator() (int start, int end) const;
};

template <typename T>
inline int
N (tm_string<T> a) {
  return a->n;
}

tm_ostream& operator<< (tm_ostream& out, tm_string_view<char> a);

template <typename T> tm_string<T>           copy (const tm_string_view<T>& a);
template <typename T> tm_string<T>           copy (tm_string<T> a);
template <typename T, size_t N> tm_string<T> copy (const T b[N]);

template <typename T> tm_string<T>& operator<< (tm_string<T>& a, T);
template <typename T>
inline tm_string<T>& operator<< (tm_string<T>& a, tm_string<T> b);
template <typename T>
inline tm_string<T>& operator<< (tm_string<T>& a, const tm_string_view<T>& b);
template <typename T, size_t Nb>
inline tm_string<T>& operator<< (tm_string<T>& a, const T (&b)[Nb]);

template <typename T>
tm_string<T>
operator* (const tm_string_view<T>& a, const tm_string_view<T>& b) {
  int          i, na= a.N, nb= b.N;
  tm_string<T> c ((int) (na + nb));
  const T *    Sa= a.a, *Sb= b.a;
  for (i= 0; i < na; i++)
    c[i]= Sa[i];
  for (i= 0; i < nb; i++)
    c[i + na]= Sb[i];
  return c;
};
template <typename T>
tm_string<T>
operator* (const tm_string_view<T>& a, tm_string<T> b) {
  return a * ((tm_string_view<T>) b);
};
template <typename T, size_t Nb>
tm_string<T>
operator* (const tm_string_view<T>& a, const T (&b)[Nb]) {
  int           i;
  constexpr int nb= Nb - 1;
  tm_string<T>  c ((int) (a.N + nb));
  const T*      Sa= a.a;
  for (i= 0; i < a.N; i++)
    c[i]= Sa[i];
  for (i= 0; i < nb; i++)
    c[i + a.N]= b[i];
  return c;
};
template <typename T>
tm_string<T>
operator* (tm_string<T> a, tm_string<T> b) {
  return ((tm_string_view<T>) a) * ((tm_string_view<T>) b);
};
template <typename T>
tm_string<T>
operator* (tm_string<T> a, const tm_string_view<T>& b) {
  return ((tm_string_view<T>) a) * b;
};
template <typename T, size_t Nb>
tm_string<T>
operator* (tm_string<T> a, const T (&b)[Nb]) {
  return ((tm_string_view<T>) a) * b;
};
template <typename T, size_t Na>
tm_string<T>
operator* (const T (&a)[Na], const tm_string_view<T>& b) {
  int           i;
  constexpr int na= Na - 1;
  tm_string<T>  c ((int) (na + b.N));
  for (i= 0; i < na; i++)
    c[i]= a[i];
  for (i= 0; i < b.N; i++)
    c[i + na]= b.a[i];
  return c;
};
template <typename T, size_t Na>
tm_string<T>
operator* (const T (&a)[Na], tm_string<T> b) {
  return a * ((tm_string_view<T>) b);
};
template <typename T, size_t Na, size_t Nb>
tm_string<T>
operator* (const T (&a)[Na], const T (&b)[Nb]) {
  int           i;
  constexpr int na= Na - 1, nb= Nb - 1;
  tm_string<T>  c ((int) (na + nb));
  for (i= 0; i < na; i++)
    c[i]= a[i];
  for (i= 0; i < nb; i++)
    c[i + na]= b[i];
  return c;
};

template <typename T>
bool
operator== (const tm_string_view<T>& a, const tm_string_view<T>& b) {
  if (a.N != b.N) return false;
  const T *Sa= a.a, *Sb= b.a;
  for (int i= 0; i < a.N; i++)
    if (Sa[i] != Sb[i]) return false;
  return true;
};
template <typename T>
bool
operator== (const tm_string_view<T>& a, tm_string<T> b) {
  return a == ((tm_string_view<T>) b);
};
template <typename T, size_t Nb>
bool
operator== (const tm_string_view<T>& a, const T (&b)[Nb]) {
  constexpr int nb= Nb - 1;
  if (a.N != nb) return false;
  const T* Sa= a.a;
  for (int i= 0; i < nb; i++)
    if (Sa[i] != b[i]) return false;
  return true;
};
template <typename T>
bool
operator== (tm_string<T> a, tm_string<T> b) {
  return ((tm_string_view<T>) a) == ((tm_string_view<T>) b);
};
template <typename T>
bool
operator== (tm_string<T> a, const tm_string_view<T>& b) {
  return ((tm_string_view<T>) a) == b;
};
template <typename T, size_t Nb>
bool
operator== (tm_string<T> a, const T (&b)[Nb]) {
  return ((tm_string_view<T>) a) == b;
};
template <typename T, size_t Na>
bool
operator== (const T (&a)[Na], const tm_string_view<T>& b) {
  constexpr int na= Na - 1;
  if (na != b.N) return false;
  const T* Sb= b.a;
  for (int i= 0; i < na; i++)
    if (a[i] != Sb[i]) return false;
  return true;
};
template <typename T, size_t Na>
bool
operator== (const T (&a)[Na], tm_string<T> b) {
  return a == ((tm_string_view<T>) b);
};

template <typename T>
bool
operator!= (const tm_string_view<T>& a, const tm_string_view<T>& b) {
  if (a.N != b.N) return true;
  const T *Sa= a.a, *Sb= b.a;
  for (int i= 0; i < a.N; i++)
    if (Sa[i] != Sb[i]) return true;
  return false;
};
template <typename T>
bool
operator!= (const tm_string_view<T>& a, tm_string<T> b) {
  return a != ((tm_string_view<T>) b);
};
template <typename T, size_t Nb>
bool
operator!= (const tm_string_view<T>& a, const T (&b)[Nb]) {
  constexpr int nb= Nb - 1;
  if (a.N != nb) return true;
  const T* Sa= a.a;
  for (int i= 0; i < nb; i++)
    if (Sa[i] != b[i]) return true;
  return false;
};
template <typename T>
bool
operator!= (tm_string<T> a, tm_string<T> b) {
  return ((tm_string_view<T>) a) != ((tm_string_view<T>) b);
};
template <typename T>
bool
operator!= (tm_string<T> a, const tm_string_view<T>& b) {
  return ((tm_string_view<T>) a) != b;
};
template <typename T, size_t Nb>
bool
operator!= (tm_string<T> a, const T (&b)[Nb]) {
  return ((tm_string_view<T>) a) != b;
};
template <typename T, size_t Na>
bool
operator!= (const T (&a)[Na], const tm_string_view<T>& b) {
  constexpr int na= Na - 1;
  const T*      Sb= b.a;
  if (na != b.N) return true;
  for (int i= 0; i < na; i++)
    if (a[i] != Sb[i]) return true;
  return false;
};
template <typename T, size_t Na>
bool
operator!= (const T (&a)[Na], tm_string<T> b) {
  return a != ((tm_string_view<T>) b);
};

template <typename T>
bool
operator< (const tm_string_view<T>& a, const tm_string_view<T>& b) {
  int      i, na= a.N, nb= b.N, nmin= min (na, nb);
  const T *Sa= a.a, *Sb= b.a;
  for (i= 0; i < nmin; i++) {
    if (Sa[i] < Sb[i]) return true;
    if (Sb[i] < Sa[i]) return false;
  }
  return na < nb;
};
template <typename T>
bool
operator< (const tm_string_view<T>& a, tm_string<T> b) {
  return a < ((tm_string_view<T>) b);
};
template <typename T, size_t Nb>
bool
operator< (const tm_string_view<T>& a, const T (&b)[Nb]) {
  constexpr int nb= Nb - 1;
  int           i, na= a.N, nmin= min (na, nb);
  const T*      Sa= a.a;
  for (i= 0; i < nmin; i++) {
    if (Sa[i] < b[i]) return true;
    if (b[i] < Sa[i]) return false;
  }
  return na < nb;
};
template <typename T>
bool
operator< (tm_string<T> a, tm_string<T> b) {
  return ((tm_string_view<T>) a) < ((tm_string_view<T>) b);
};
template <typename T>
bool operator< (tm_string<T>             a,
                const tm_string_view<T>& b){return ((tm_string_view<T>) a) < b};
template <typename T, size_t Nb>
bool operator< (tm_string<T> a,
                const T (&b)[Nb]){return ((tm_string_view<T>) a) < b};
template <typename T, size_t Na>
bool
operator< (const T (&a)[Na], const tm_string_view<T>& b) {
  constexpr int na= Na - 1;
  int           i, nb= b.N, nmin= min (na, nb);
  const T*      Sb= b.a;
  for (i= 0; i < nmin; i++) {
    if (a[i] < Sb[i]) return true;
    if (Sb[i] < a[i]) return false;
  }
  return na < nb;
};
template <typename T, size_t Na>
bool
operator< (const T (&a)[Na], tm_string<T> b) {
  return a < ((tm_string_view<T>) b);
};

template <typename T>
bool
operator<= (const tm_string_view<T>& a, const tm_string_view<T>& b) {
  int      i, na= a.N, nb= b.N, nmin= min (na, nb);
  const T *Sa= a.a, *Sb= b.a;
  for (i= 0; i < nmin; i++) {
    if (Sa[i] < Sb[i]) return true;
    if (Sb[i] < Sa[i]) return false;
  }
  return na <= nb;
};
template <typename T>
bool
operator<= (const tm_string_view<T>& a, tm_string<T> b) {
  return a <= ((tm_string_view<T>) b);
};
template <typename T, size_t Nb>
bool
operator<= (const tm_string_view<T>& a, const T (&b)[Nb]) {
  constexpr int nb= Nb - 1;
  int           i, na= a.N, nmin= min (na, nb);
  const T*      Sa= a.a;
  for (i= 0; i < nmin; i++) {
    if (Sa[i] < b[i]) return true;
    if (b[i] < Sa[i]) return false;
  }
  return na <= nb;
};
template <typename T>
bool
operator<= (tm_string<T> a, tm_string<T> b) {
  return ((tm_string_view<T>) a) <= ((tm_string_view<T>) b);
};
template <typename T>
bool
operator<= (tm_string<T> a, const tm_string_view<T>& b) {
  return ((tm_string_view<T>) a) <= b;
};
template <typename T, size_t Nb>
bool
operator<= (tm_string<T> a, const T (&b)[Nb]) {
  return ((tm_string_view<T>) a) <= b;
};
template <typename T, size_t Na>
bool
operator<= (const T (&a)[Na], const tm_string_view<T>& b) {
  constexpr int na= Na - 1;
  int           i, nb= b.N, nmin= min (na, nb);
  const T*      Sb= b.a;
  for (i= 0; i < nmin; i++) {
    if (a[i] < Sb[i]) return true;
    if (Sb[i] < a[i]) return false;
  }
  return na <= nb;
};
template <typename T, size_t Na>
bool
operator<= (const T (&a)[Na], tm_string<T> b) {
  return a <= ((tm_string_view<T>) b);
};

template <typename T> int hash (tm_string_view<T> s);
template <typename T> int hash (tm_string<T> s);
} // namespace data
} // namespace lolly

#include "tm_string.ipp"
