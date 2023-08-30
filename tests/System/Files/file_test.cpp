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

TEST_CASE ("is_directory") {
  if (!tb_init (tb_null, tb_null)) exit (-1);
  CHECK (is_directory (string ("C:/Windows")));
}
