
/******************************************************************************
 * MODULE     : sys_utils.cpp
 * DESCRIPTION: file handling
 * COPYRIGHT  : (C) 1999-2016  Joris van der Hoeven, Denis Raux
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

// #include "config.h"
#include "sys_utils.hpp"
// #include "file.hpp"
#include "parse_string.hpp"
#include "tree.hpp"

#ifdef OS_MINGW
#include "mingw_sys_utils.hpp"
#include "win-utf8-compat.hpp"
#elif !(defined OS_WIN32)
#include "unix_sys_utils.hpp"
#endif

string
get_env (string var) {
  c_string    _var (var);
  const char* _ret= getenv (_var);
  if (_ret == NULL) {
    if (var == "PWD") return get_env ("HOME");
    return "";
  }
  string ret (_ret);
  return ret;
  // do not delete _ret !
}