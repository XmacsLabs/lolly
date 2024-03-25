
/** \file string_u16.cpp
 *  \copyright GPLv3
 *  \details Strings with different type of char, and corresponding readonly
 *           view of string. Zero-characters are allowed in strings.
 *  \author jingkaimori
 *  \date   2024
 */

#pragma once

#include "string_u16.hpp"
#include "minmax.hpp"

namespace lolly {
namespace data {

static inline constexpr int
round_length (int n) {
  n= (n + 3) & (0xfffffffc);
  if (n < 24) return n;
  int i= 32;
  while (n > i)
    i<<= 1;
  return i;
}

string_u16_rep::string_u16_rep (int n2)
    : n (n2), a_N (round_length (n)),
      a ((n == 0) ? ((char16_t*) NULL) : tm_new_array<char16_t> (a_N)) {}

void
string_u16_rep::resize (int m) {
  int nn= round_length (n);
  int mm= round_length (m);
  if (mm != nn) {
    if (mm != 0) {
      if (nn != 0) {
        a_N= mm;
        a  = tm_resize_array<char16_t> (mm, a);
      }
      else {
        a_N= mm;
        a  = tm_new_array<char16_t> (mm);
      }
    }
    else if (nn != 0) {
      tm_delete_array (a);
      a_N= 0;
      a  = NULL;
    };
  }
  n= m;
}

int
string_u16_rep::expand_or_shrink_by (int delta) {
  int old_n= n;
  n+= delta;
  reserve (n);
  return old_n;
}

void
string_u16_rep::reserve (int new_n) {
  int old_size= a_N;
  if (new_n != 0) {
    if (old_size == 0) {
      a_N= round_length (new_n);
      a  = tm_new_array<char16_t> (a_N);
    }
    else if (old_size < new_n) {
      a_N= round_length (new_n);
      a  = tm_resize_array<char16_t> (a_N, a);
    }
  }
  else {
    if (old_size != 0) {
      tm_delete_array (a);
      a  = NULL;
      a_N= 0;
    };
  }
}

string_u16::string_u16 (char16_t c) : rep (tm_new<string_u16_rep> (1)) {
  rep->a[0]= c;
};

string_u16::string_u16 (const string_u16_view& c)
    : rep (tm_new<string_u16_rep> (c.N)) {
  for (int i= 0; i < c.N; i++)
    rep->a[i]= c.a[i];
};

string_u16::string_u16 (char16_t c, int n) : rep (tm_new<string_u16_rep> (n)) {
  for (int i= 0; i < n; i++)
    rep->a[i]= c;
};

template <size_t N>
string_u16::string_u16 (const char16_t (&s)[N])
    : rep (tm_new<string_u16_rep> (N - 1)) {
  constexpr int n= N - 1;
  for (int i= 0; i < n; i++)
    rep->a[i]= s[i];
};

string_u16_view
string_u16::operator() (int start, int end) {
  if (end <= start) {
    return string_u16_view (0, nullptr);
  }
  start= max (min (rep->n, start), 0);
  end  = max (min (rep->n, end), 0);
  return string_u16_view (end - start, rep->a + start);
}

// string_u16
// copy (const string_u16_view& a) {
//   int        i;
//   string_u16 r (a.N);
//   for (i= 0; i < a.N; i++)
//     r[i]= a.a[i];
//   return r;
// };
//
// string_u16
// copy (string_u16 a) {
//   int        i, n= N (a);
//   string_u16 r (a);
//   for (i= 0; i < a; i++)
//     r[i]= a[i];
//   return r;
// };
//
// template <size_t N>
// string_u16
// copy (const T a[N]) {
//   int    i;
//   string r ((int) N);
//   for (i= 0; i < N; i++)
//     r[i]= a[i];
//   return r;
// };
//
// string_u16&
// operator<< (string_u16& a, T ch) {
//   int na= a->expand_or_shrink_by (1);
//   a[na] = ch;
//   return a;
// };
//
// inline string_u16&
// operator<< (string_u16& a, string_u16 b) {
//   int i, nb= N (b);
//   int na= a->expand_or_shrink_by (nb);
//   for (i= 0; i < nb; i++)
//     a[i + na]= b[i];
//   return a;
// };
//
// inline string_u16&
// operator<< (string_u16& a, const string_u16& b) {
//   int i;
//   int na= a->expand_or_shrink_by (b.N);
//   for (i= 0; i < b.N; i++)
//     a[i + na]= b.a[i];
//   return a;
// };
//
// template <size_t Nb>
// inline string_u16&
// operator<< (string_u16& a, const char16_t (&b)[Nb]) {
//   int           i;
//   constexpr int nb= Nb - 1;
//   int           na= a->expand_or_shrink_by (nb);
//   for (i= 0; i < nb; i++)
//     a[i + na]= b[i];
//   return a;
// }
//
// string_u16
// operator* (const string_u16& a, string_u16 b) {
//   return a * ((string_u16_view) b);
// };
//
// string_u16
// operator* (string_u16 a, const lolly_string_view<T>& b) {
//   return ((string_u16_view) a) * b;
// };
//
// bool
// operator== (const lolly_string_view<T>& a, string_u16 b) {
//   return a == ((string_u16_view) b);
// };
//
// template <typename T, size_t Nb>
// bool
// operator== (const lolly_string_view<T>& a, const T (&b)[Nb]) {
//   constexpr int nb= Nb - 1;
//   if (a.N != nb) return false;
//   const T* Sa= a.a;
//   for (int i= 0; i < nb; i++)
//     if (Sa[i] != b[i]) return false;
//   return true;
// };
//
// bool
// operator== (string_u16 a, string_u16 b) {
//   return ((string_u16_view) a) == ((string_u16_view) b);
// };
//
// bool
// operator== (string_u16 a, const string_u16_view& b) {
//   return ((string_u16_view) a) == b;
// };
//
// template <typename T, size_t Nb>
// bool
// operator== (string_u16 a, const T (&b)[Nb]) {
//   return ((string_u16_view) a) == b;
// };
//
// template <typename T, size_t Na>
// bool
// operator== (const T (&a)[Na], string_u16 b) {
//   return a == ((string_u16_view) b);
// };
//
// bool
// operator!= (const string_u16_view& a, string_u16 b) {
//   return a != ((string_u16_view) b);
// };
//
// bool
// operator!= (string_u16 a, string_u16 b) {
//   return ((string_u16_view) a) != ((string_u16_view) b);
// };
//
// bool
// operator!= (string_u16 a, const lolly_string_view<T>& b) {
//   return ((string_u16_view) a) != b;
// };
//
// template <typename T, size_t Nb>
// bool
// operator!= (string_u16 a, const T (&b)[Nb]) {
//   return ((string_u16_view) a) != b;
// };
//
// template <typename T, size_t Na>
// bool
// operator!= (const T (&a)[Na], string_u16 b) {
//   return a != ((string_u16_view) b);
// };
//
// template <typename T>
// bool
// operator< (const lolly_string_view<T>& a, string_u16 b) {
//   return a < ((string_u16_view) b);
// };
// template <typename T>
// bool
// operator< (string_u16 a, string_u16 b) {
//   return ((string_u16_view) a) < ((string_u16_view) b);
// };
// template <typename T>
// bool
// operator< (string_u16 a, const lolly_string_view<T>& b) {
//   return ((string_u16_view) a) < b;
// };
// template <typename T, size_t Nb>
// bool
// operator< (string_u16 a, const T (&b)[Nb]) {
//   return ((string_u16_view) a) < b;
// };
// template <typename T, size_t Na>
// bool
// operator< (const T (&a)[Na], string_u16 b) {
//   return a < ((string_u16_view) b);
// };
//
// template <typename T>
// bool
// operator<= (const lolly_string_view<T>& a, string_u16 b) {
//   return a <= ((string_u16_view) b);
// };
// template <typename T>
// bool
// operator<= (string_u16 a, string_u16 b) {
//   return ((string_u16_view) a) <= ((string_u16_view) b);
// };
// template <typename T>
// bool
// operator<= (string_u16 a, const lolly_string_view<T>& b) {
//   return ((string_u16_view) a) <= b;
// };
// template <typename T, size_t Nb>
// bool
// operator<= (string_u16 a, const T (&b)[Nb]) {
//   return ((string_u16_view) a) <= b;
// };
// template <typename T, size_t Na>
// bool
// operator<= (const T (&a)[Na], string_u16 b) {
//   return a <= ((string_u16_view) b);
// };
//
// template <typename T>
// int
// hash (lolly_string_view<T> s) {
//   int i, h= 0, n= s.N;
//   for (i= 0; i < n; i++) {
//     h= (h << 9) + (h >> 23);
//     h= h + ((int) s.a[i]);
//   }
//   return h;
// };
} // namespace data
} // namespace lolly
