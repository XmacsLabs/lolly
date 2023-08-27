
/******************************************************************************
 * MODULE     : mingw_sys_utils.cpp
 * DESCRIPTION: external command handling
 * COPYRIGHT  : (C) 2015  Gregoire LECERF, Denis RAUX
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "win_sys_utils.hpp"
#include "analyze.hpp"
#include "tm_timer.hpp"
#include "win_utf8_compat.hpp"

namespace lolly {

#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif

#ifdef OS_WIN
#include <windows.h>
#endif
#include <windef.h>

#include <basetsd.h>
#include <ntsecapi.h>
#ifdef OS_WIN
#include <security.h>
#else
#include <secext.h>
#endif
#include <wtypesbase.h>

string
win_get_username () {
  const int MAX_LEN= 100;
  WCHAR     buffer[MAX_LEN];
  DWORD     len;

  // This API must be called twice, otherwise it returns an empty use name
  GetUserNameExW (NameDisplay, buffer, &len);
  GetUserNameExW (NameDisplay, buffer, &len);

  if (len == 0) return string ("");
  else return string (nowide::narrow (buffer).c_str ());
}

} // namespace lolly
