
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

#ifndef STRING_H
#define STRING_H

#include "lolly/data/lolly_string.hpp"
#include <stdint.h>

using lolly::data::string;
using string_view= lolly::data::lolly_string_view<char>;

bool     as_bool (string s);
int      as_int (string s);
long int as_long_int (string s);
double   as_double (string s);
char*    as_charp (string s);
string   as_string_bool (bool f);
string   as_string (int16_t i);
string   as_string (int32_t i);
string   as_string (int64_t i);
string   as_string (unsigned int i);
string   as_string (unsigned long int i);
string   as_string (double x);
string   as_string (const char* s);
bool     is_empty (string s);
bool     is_bool (string s);
bool     is_int (string s);
bool     is_double (string s);
bool     is_charp (string s);

bool is_quoted (string s);
bool is_id (string s);

/**
 * @brief C-style strings with automatic memory management. NUL is appended to
 * the end of string
 *
 * @note Use this interface instead of as_charp whenever possible.
 */
class c_string;
class c_string_rep : concrete_struct {
  char* value;

private:
  inline c_string_rep (c_string_rep&) : concrete_struct () {}
  // disable copy constructor
  inline c_string_rep& operator= (c_string_rep&) { return *this; }
  // disable assignment

public:
  inline c_string_rep (char* v= NULL) : value (v) {}
  inline ~c_string_rep () {
    if (value != NULL) tm_delete_array (value);
  }
  friend class c_string;
};

class c_string {
  CONCRETE (c_string);

public:
  inline c_string () : rep (tm_new<c_string_rep> ()) {}
  inline c_string (int len)
      : rep (tm_new<c_string_rep> (tm_new_array<char> (len))) {}
  c_string (string s);
  inline operator char* () const { return rep->value; }
};
CONCRETE_CODE (c_string);

#endif // defined STRING_H
