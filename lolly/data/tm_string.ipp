
#pragma once
#include "tm_string.hpp"
namespace lolly {
namespace data {

static inline int
round_length (int n) {
  n= (n + 3) & (0xfffffffc);
  if (n < 24) return n;
  int i= 32;
  while (n > i)
    i<<= 1;
  return i;
}

template <class T>
tm_string_rep<T>::tm_string_rep (int n2)
    : n (n2), a ((n == 0) ? ((T*) NULL) : tm_new_array<T> (round_length (n))) {}

template <class T>
void
tm_string_rep<T>::resize (int m) {
  int nn= round_length (n);
  int mm= round_length (m);
  if (mm != nn) {
    if (mm != 0) {
      if (nn != 0) {
        a= tm_resize_array<T> (mm, a);
      }
      else {
        a= tm_new_array<T> (mm);
      }
    }
    else if (nn != 0) tm_delete_array (a);
  }
  n= m;
}

template <class T>
tm_string<T>::tm_string (T c) : rep (tm_new<tm_string_rep<T>> (1)) {
  rep->a[0]= c;
};

template <class T>
tm_string<T>::tm_string (const tm_string_view<T>& c)
    : rep (tm_new<tm_string_rep<T>> (c.N)) {
  for (int i= 0; i < c.N; i++)
    rep->a[i]= c.a[i];
};

template <class T>
tm_string<T>::tm_string (T c, int n) : rep (tm_new<tm_string_rep<T>> (n)) {
  for (int i= 0; i < n; i++)
    rep->a[i]= c;
};
template <class T>
template <size_t N>
tm_string<T>::tm_string (const T (&s)[N])
    : rep (tm_new<tm_string_rep<T>> (N - 1)) {
  constexpr int n= N - 1;
  for (int i= 0; i < n; i++)
    rep->a[i]= s[i];
};
template <class T>
tm_string_view<T>
tm_string<T>::operator() (int start, int end) {
  if (end <= start) {
    return tm_string_view<T> (0, nullptr);
  }
  start= max (min (rep->n, start), 0);
  end  = max (min (rep->n, end), 0);
  return tm_string_view<T> (end - start, rep->a + start);
}
template <class T>
tm_string_view<T>
tm_string_view<T>::operator() (int start, int end) const {
  if (end <= start) {
    return tm_string_view<T> (0, nullptr);
  }
  start= max (min (N, start), 0);
  end  = max (min (N, end), 0);
  return tm_string_view<T> (end - start, a + start);
}

template <typename T>
tm_string<T>
copy (tm_string_view<T> a) {
  int    i;
  string r (a.N);
  for (i= 0; i < a.N; i++)
    r[i]= a.a[i];
  return r;
};

template <typename T>
tm_string<T>
copy (tm_string<T> a) {
  int    i, n= N (a);
  string r (a);
  for (i= 0; i < a; i++)
    r[i]= a[i];
  return r;
};

template <typename T, size_t N>
tm_string<T>
copy (const T a[N]) {
  int    i;
  string r (N);
  for (i= 0; i < N; i++)
    r[i]= a[i];
  return r;
};

tm_ostream&
operator<< (tm_ostream& out, tm_string_view<char> a) {
  int i, n= a.N;
  if (n == 0) return out;
  for (i= 0; i < n; i++)
    out << a.a[i];
  return out;
};

template <typename T>
tm_string<T>&
operator<< (tm_string<T>& a, T ch) {
  int i, na= N (a);
  a->resize (na + 1);
  a[na]= ch;
  return a;
};

template <typename T>
inline tm_string<T>&
operator<< (tm_string<T>& a, tm_string<T> b) {
  int i, na= N (a), nb= N (b);
  a->resize (na + nb);
  for (i= 0; i < nb; i++)
    a[i + na]= b[i];
  return a;
};

template <typename T>
inline tm_string<T>&
operator<< (tm_string<T>& a, const tm_string_view<T>& b) {
  int i, na= N (a);
  a->resize (na + b.N);
  for (i= 0; i < b.N; i++)
    a[i + na]= b.a[i];
  return a;
};

template <typename T, size_t Nb>
inline tm_string<T>&
operator<< (tm_string<T>& a, const T (&b)[Nb]) {
  int           i, na= N (a);
  constexpr int nb= Nb - 1;
  a->resize (na + nb);
  for (i= 0; i < nb; i++)
    a[i + na]= b[i];
  return a;
}

template <typename T>
int
hash (tm_string<T> s) {
  int i, h= 0, n= N (s);
  for (i= 0; i < n; i++) {
    h= (h << 9) + (h >> 23);
    h= h + ((int) s[i]);
  }
  return h;
};
template <typename T>
int
hash (tm_string_view<T> s) {
  int i, h= 0, n= s.N;
  for (i= 0; i < n; i++) {
    h= (h << 9) + (h >> 23);
    h= h + ((int) s.a[i]);
  }
  return h;
};
} // namespace data
} // namespace lolly