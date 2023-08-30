/** \file url.cpp
 *  \copyright GPLv3
 *  \details Unitests for url.
 *  \author Darcy
 *  \date   2019-2023
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "url.hpp"

TEST_CASE ("suffix") {
  // empty suffix should work
  url no_suffix= url ("/a/b/c/d/no_suffix");
  CHECK (is_empty (suffix (no_suffix)));
  url no_suffix2= url ("/a/b.c/d/no_suffix");
  CHECK (is_empty (suffix (no_suffix2)));

  // normal suffix should work
  url png= url ("/a/b/c/d.png");
  CHECK_EQ (suffix (png) == string ("png"), true);
  url png2= url ("/a/b.c/d.png");
  CHECK_EQ (suffix (png2) == string ("png"), true);
}
