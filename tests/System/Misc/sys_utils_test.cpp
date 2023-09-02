#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "sys_utils.hpp"
#include "tbox/tbox.h"

TEST_CASE ("system") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
#ifdef OS_WASM
  CHECK_EQ (lolly::system ("xmake --version"), -1);
  CHECK_EQ (lolly::system ("no_such_command"), -1);
  CHECK_EQ (lolly::system (""), -1);
#else
#ifndef OS_MINGW
  CHECK (lolly::system ("xmake --version") == 0);
#endif
  CHECK (lolly::system ("no_such_command") != 0);
  CHECK (lolly::system ("") != 0);
#endif
}

TEST_CASE ("get_env/set_env") {
#if defined(OS_MINGW) || defined(OS_WIN)
  CHECK_EQ (get_env ("SystemRoot") == "C:\\WINDOWS", true);
  CHECK_EQ (get_env ("ProgramFiles") == "C:\\Program Files", true);
#endif

#if defined(OS_LINUX)
  CHECK_EQ (get_env ("SHELL") == "/bin/bash", true);
#endif
}
