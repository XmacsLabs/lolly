#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "sys_utils.hpp"
#include <cstdlib>

TEST_CASE ("system") {
#ifdef OS_WASM
  CHECK_EQ (lolly::system ("xmake --version"), -1);
  CHECK_EQ (lolly::system ("no_such_command"), -1);
  CHECK_EQ (lolly::system (""), -1);
#else
  CHECK (lolly::system ("xmake --version") == 0);
  CHECK (lolly::system ("no_such_command") != 0);
  CHECK (lolly::system ("") == 0);
#endif
}
