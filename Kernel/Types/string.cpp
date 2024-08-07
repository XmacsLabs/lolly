
/******************************************************************************
 * MODULE     : string.cpp
 * DESCRIPTION: Fixed size strings with reference counting.
 *              Zero characters can be part of string.
 * COPYRIGHT  : (C) 1999  Joris van der Hoeven
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "string.hpp"
#include "basic.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

/******************************************************************************
 * Low level routines and constructors
 ******************************************************************************/

static inline int
round_length (int n) {
  n= (n + 3) & (0xfffffffc);
  if (n < 24) return n;
  int i= 32;
  while (n > i)
    i<<= 1;
  return i;
}

string_rep::string_rep (int n2) : n (n2), a_N (round_length (n2)), a (nullptr) {
  if (a_N != 0) {
    a= tm_new_array<char> (a_N);
  }
}

void
string_rep::resize (int m) {
  int nn= round_length (n);
  int mm= round_length (m);
  if (mm != nn) {
    if (mm != 0) {
      if (nn != 0) {
        a_N= mm;
        a  = tm_resize_array<char> (mm, a);
      }
      else {
        a_N= mm;
        a  = tm_new_array<char> (mm);
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
string_rep::expand_or_shrink_by (int delta) {
  int old_n= n;
  n+= delta;
  reserve (n);
  return old_n;
}

void
string_rep::reserve (int new_n) {
  int old_size= a_N;
  if (new_n != 0) {
    if (old_size == 0) {
      a_N= round_length (new_n);
      a  = tm_new_array<char> (a_N);
    }
    else if (old_size < new_n) {
      a_N= round_length (new_n);
      a  = tm_resize_array<char> (a_N, a);
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

string::string (char c) {
  rep      = tm_new<string_rep> (1);
  rep->a[0]= c;
}

string::string (char c, int n) {
  rep= tm_new<string_rep> (n);
  for (int i= 0; i < n; i++)
    rep->a[i]= c;
}

string::string (const char* a) {
  int i, n= strlen (a);
  rep= tm_new<string_rep> (n);
  for (i= 0; i < n; i++)
    rep->a[i]= a[i];
}

string::string (const char* a, int n) {
  int i;
  rep= tm_new<string_rep> (n);
  for (i= 0; i < n; i++)
    rep->a[i]= a[i];
}

/******************************************************************************
 * Common routines for strings
 ******************************************************************************/

bool
string::operator== (const char* s) {
  int   i, n= rep->n;
  char* S= rep->a;
  for (i= 0; i < n; i++) {
    if (s[i] != S[i]) return false;
    if (s[i] == '\0') return false;
  }
  return (s[i] == '\0');
}

bool
string::operator!= (const char* s) {
  int   i, n= rep->n;
  char* S= rep->a;
  for (i= 0; i < n; i++) {
    if (s[i] != S[i]) return true;
    if (s[i] == '\0') return true;
  }
  return (s[i] != '\0');
}

bool
string::operator== (string a) {
  return ((string_view<char>) *this) == ((string_view<char>) a);
}

bool
string::operator!= (string a) {
  return ((string_view<char>) *this) != ((string_view<char>) a);
}

string
string::operator() (int begin, int end) {
  if (end <= begin) return string ();

  int i;
  begin= max (min (rep->n, begin), 0);
  end  = max (min (rep->n, end), 0);
  string r (end - begin);
  for (i= begin; i < end; i++)
    r[i - begin]= rep->a[i];
  return r;
}

string
copy (string s) {
  int    i, n= N (s);
  string r (n);
  for (i= 0; i < n; i++)
    r[i]= s[i];
  return r;
}

string&
operator<< (string& a, char x) {
  int old_a_N= a->expand_or_shrink_by (1);
  a[old_a_N] = x;
  return a;
}

string&
operator<< (string& a, string b) {
  int b_N    = N (b);
  int old_a_N= a->expand_or_shrink_by (b_N);
  for (int i= 0; i < b_N; i++)
    a[i + old_a_N]= b[i];
  return a;
}

string
operator* (string a, string b) {
  int    i, n1= N (a), n2= N (b);
  string c (n1 + n2);
  for (i= 0; i < n1; i++)
    c[i]= a[i];
  for (i= 0; i < n2; i++)
    c[i + n1]= b[i];
  return c;
}

string
operator* (const char* a, string b) {
  return string (a) * b;
}

string
operator* (string a, const char* b) {
  return a * string (b);
}

bool
operator< (string s1, string s2) {
  return ((string_view<char>) s1) < ((string_view<char>) s2);
}

bool
operator<= (string s1, string s2) {
  return ((string_view<char>) s1) <= ((string_view<char>) s2);
}

int
hash (string s) {
  int h= 0;
  for (char ch : s) {
    h= (h << 9) + (h >> 23);
    h= h + ((int) ch);
  }
  return h;
}

/******************************************************************************
 * Conversion routines
 ******************************************************************************/

bool
as_bool (string s) {
  return (s == "true" || s == "#t");
}

int
as_int (string s) {
  int i= 0, n= N (s), val= 0;
  if (n == 0) return 0;
  if (s[0] == '+') i++;
  if (s[0] == '-') i++;
  while (i < n) {
    if (s[i] < '0') break;
    if (s[i] > '9') break;
    val*= 10;
    val+= (int) (s[i] - '0');
    i++;
  }
  if (s[0] == '-') val= -val;
  return val;
}

long int
as_long_int (string s) {
  int      i= 0, n= N (s);
  long int val= 0;
  if (n == 0) return 0;
  if (s[0] == '+') i++;
  if (s[0] == '-') i++;
  while (i < n) {
    if (s[i] < '0') break;
    if (s[i] > '9') break;
    val*= 10;
    val+= (int) (s[i] - '0');
    i++;
  }
  if (s[0] == '-') val= -val;
  return val;
}

double
as_double (string s) {
  double x= 0.0;
  {
    int i, n= N (s);
    STACK_NEW_ARRAY (buf, char, n + 1);
    for (i= 0; i < n; i++)
      buf[i]= s[i];
    buf[n]= '\0';
    sscanf (buf, "%lf", &x);
    STACK_DELETE_ARRAY (buf);
  } // in order to avoid segmentation fault due to compiler bug
  return x;
}

char*
as_charp (string s) {
  int   i, n= N (s);
  char* s2= tm_new_array<char> (n + 1);
  for (i= 0; i < n; i++)
    s2[i]= s[i];
  s2[n]= '\0';
  return s2;
}

string
as_string_bool (bool f) {
  if (f) return string ("true");
  else return string ("false");
}

string
as_string (int16_t i) {
  return string ((std::to_string (i)).c_str ());
}

string
as_string (int32_t i) {
  return string ((std::to_string (i)).c_str ());
}

string
as_string (int64_t i) {
  return string ((std::to_string (i)).c_str ());
}

string
as_string (unsigned int i) {
  char buf[64];
  sprintf (buf, "%u", i);
  // sprintf (buf, "%u\0", i);
  return string (buf);
}

string
as_string (unsigned long int i) {
  char buf[64];
  sprintf (buf, "%lu", i);
  // sprintf (buf, "%lu\0", i);
  return string (buf);
}

string
as_string (double x) {
  char buf[64];
  sprintf (buf, "%g", x);
  // sprintf (buf, "%g\0", x);
  return string (buf);
}

string
as_string (const char* s) {
  return string (s);
}

bool
is_empty (string s) {
  return N (s) == 0;
}

bool
is_bool (string s) {
  return (s == "true") || (s == "false");
}

bool
is_int (string s) {
  int i= 0, n= N (s);
  if (n == 0) return false;
  if (s[i] == '+') i++;
  if (s[i] == '-') i++;
  if (i == n) return false;
  for (; i < n; i++)
    if ((s[i] < '0') || (s[i] > '9')) return false;
  return true;
}

bool
is_double (string s) {
  int i= 0, n= N (s);
  if (n == 0) return false;
  if (s[i] == '+') i++;
  if (s[i] == '-') i++;
  if (i == n) return false;
  for (; i < n; i++)
    if ((s[i] < '0') || (s[i] > '9')) break;
  if (i == n) return true;
  if (s[i] == '.') {
    i++;
    if (i == n) return false;
    for (; i < n; i++)
      if ((s[i] < '0') || (s[i] > '9')) break;
  }
  if (i == n) return true;
  if (s[i++] != 'e') return false;
  if (s[i] == '+') i++;
  if (s[i] == '-') i++;
  if (i == n) return false;
  for (; i < n; i++)
    if ((s[i] < '0') || (s[i] > '9')) return false;
  return true;
}

bool
is_charp (string s) {
  (void) s;
  return true;
}

bool
is_quoted (string s) {
  int n= N (s);
  return (n >= 2) && (s[0] == '\"') && (s[n - 1] == '\"');
}

bool
is_id (string s) {
  int i= 0, n= N (s);
  if (n == 0) return false;
  for (i= 0; i < n; i++) {
    if ((i > 0) && (s[i] >= '0') && (s[i] <= '9')) continue;
    if ((s[i] >= 'a') && (s[i] <= 'z')) continue;
    if ((s[i] >= 'A') && (s[i] <= 'Z')) continue;
    if (s[i] == '_') continue;
    return false;
  }
  return true;
}
