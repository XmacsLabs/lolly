
/******************************************************************************
 * MODULE     : file_url.cpp
 * DESCRIPTION: url rooted with file://
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "url.hpp"

file_url::file_url (string name)
    : url (url_root ("file") * url_get_name (name)) {}

file_url::file_url (const char* name)
    : url (url_root ("file") * url_get_name (string (name))) {}

string
file_url::concretize () {
  url c= reroot (*this, "default");
  if (!is_none (c)) return as_string (c);
  if (is_wildcard (*this, 1)) return (*this)->t[1]->label;
  TM_FAILED ("failed to concretize file_url");
}
