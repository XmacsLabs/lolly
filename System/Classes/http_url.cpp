
/******************************************************************************
 * MODULE     : http_url.cpp
 * DESCRIPTION: url rooted with http://
 * COPYRIGHT  : (C) 1999  Joris van der Hoeven
 *                  2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "url.hpp"

http_url::http_url (string name)
    : url (url_root ("http") * url_get_name (name)) {}

http_url::http_url (const char* name)
    : url (url_root ("http") * url_get_name (as_string (name))) {}
