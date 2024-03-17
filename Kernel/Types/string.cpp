
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
#include <charconv>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <system_error>

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

using string_view= lolly::data::lolly_string_view<char>;

string
as_string (int16_t i) {
  char buffer[6];
  auto [ptr, ec]= std::to_chars (buffer, buffer + 6, i);
  if (ec == std::errc ()) {
    return string (string_view (ptr - buffer, buffer));
  }
  else {
    return string ();
  }
}

string
as_string (int32_t i) {
  char buffer[11];
  auto [ptr, ec]= std::to_chars (buffer, buffer + 11, i);
  if (ec == std::errc ()) {
    return string (string_view (ptr - buffer, buffer));
  }
  else {
    return string ();
  }
}

string
as_string (int64_t i) {
  char buffer[20];
  auto [ptr, ec]= std::to_chars (buffer, buffer + 20, i);
  if (ec == std::errc ()) {
    return string (string_view (ptr - buffer, buffer));
  }
  else {
    return string ();
  }
}

string
as_string (unsigned int i) {
  char buffer[32];
  auto [ptr, ec]= std::to_chars (buffer, buffer + 32, i);
  if (ec == std::errc ()) {
    return string (string_view (ptr - buffer, buffer));
  }
  else {
    return string ();
  }
}

string
as_string (unsigned long int i) {
  char buffer[32];
  auto [ptr, ec]= std::to_chars (buffer, buffer + 32, i);
  if (ec == std::errc ()) {
    return string (string_view (ptr - buffer, buffer));
  }
  else {
    return string ();
  }
}

string
as_string (double x) {
  char buffer[64];
  auto [ptr, ec]= std::to_chars (buffer, buffer + 32, x);
  if (ec == std::errc ()) {
    return string (string_view (ptr - buffer, buffer));
  }
  else {
    return string ();
  }
}

string
as_string (const char* s) {
  return string (string_view (strlen (s), s));
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

c_string::c_string (string s) : rep (tm_new<c_string_rep> (as_charp (s))) {}
