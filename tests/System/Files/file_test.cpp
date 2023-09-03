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

#if defined(OS_LINUX)
TEST_CASE ("is_symbolic_link on linux") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
  CHECK (is_symbolic_link (url_system ("/usr/bin/python")));
}
#endif

TEST_CASE ("is_directory/is_regular") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
  CHECK (is_directory (url_pwd ()));
  CHECK (!is_regular (url_pwd ()));
  CHECK (!is_symbolic_link (url_pwd ()));

  url xmake_lua= url_pwd () * url ("xmake.lua");
  CHECK (!is_directory (xmake_lua));
  CHECK (is_regular (xmake_lua));
  CHECK (!is_symbolic_link (xmake_lua));
}

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

TEST_CASE ("file_size") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
  CHECK (file_size (url_pwd () * url ("xmake.lua")) > 0);
}

TEST_CASE ("read_directory") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
  bool flag1= false;
  CHECK (N (read_directory (url_pwd (), flag1)) > 0);
  CHECK (!flag1); // no error
  bool flag2= false;
  CHECK (N (read_directory (url_system ("no_such_dir"), flag2)) == 0);
  CHECK (flag2); // error
}
