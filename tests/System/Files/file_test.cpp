/** \file file.cpp
 *  \copyright GPLv3
 *  \details Unitests for file.
 *  \author Darcy
 *  \date   2019-2023
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "file.hpp"
#include "tbox/tbox.h"

#if defined(OS_WIN) || defined(OS_MINGW)
TEST_CASE ("is_directory on Windows") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
  CHECK (is_directory (url_system ("C:/Windows")));
}
#endif

TEST_CASE ("mkdir/rmdir") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
#if defined(OS_WIN) || defined(OS_MINGW)
  url test_mkdir= url_system ("$TMP") * url ("lolly_tmp_dir");
#else
  url test_mkdir= url_system ("/tmp") * url ("lolly_tmp_dir");
#endif
  mkdir (test_mkdir);
  CHECK (is_directory (test_mkdir));
  rmdir (test_mkdir);
  CHECK (!is_directory (test_mkdir));
}
