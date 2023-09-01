/** \file url.cpp
 *  \copyright GPLv3
 *  \details Unitests for url.
 *  \author Darcy
 *  \date   2019-2023
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "url.hpp"

url win_c_windows= url_system ("C:/Windows");
url win_c        = url_system ("C:/");
url system_root  = url_system ("%SystemRoot%");
url windir       = url_system ("%windir%");
url unix_root    = url_system ("/");

#if defined(OS_MINGW) || defined(OS_WIN)
TEST_CASE ("as_string on windows") {
#ifdef OS_WIN
  CHECK_EQ (as_string (win_c) == string ("C:{/}"), true);
#else
  CHECK_EQ (as_string (win_c) == string ("C:\\"), true);
#endif
#ifdef OS_WIN
  CHECK_EQ (as_string (win_c_windows) == string ("C:/Windows"), true);
#else
  CHECK_EQ (as_string (win_c_windows) == string ("C:\\Windows"), true);
#endif
  CHECK_EQ (as_string (system_root) == string ("%SystemRoot%"), true);
  CHECK_EQ (as_string (windir) == string ("%windir%"), true);
}
#endif

TEST_CASE ("is_root") {
  CHECK (is_root (unix_root));
  CHECK (!is_root (win_c));
}
