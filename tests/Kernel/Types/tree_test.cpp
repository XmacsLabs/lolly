#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "tree.hpp"

TEST_CASE ("test_is_atomic") {
  CHECK (is_atomic (tree ()));
  CHECK (is_atomic (tree (0, tree ())));
  CHECK (!is_atomic (tree (1, tree (), tree ())));
  CHECK (!is_atomic (tree (2, tree (), tree ())));
}

TEST_CASE ("test_is_compound") {
  CHECK (is_compound (tree (70, 3)));
  CHECK (is_compound (tree (1, tree ())));
  CHECK (!is_compound (tree ()));
  CHECK (!is_compound (tree (0, tree ())));
}

TEST_CASE ("test_is_bool") {
  CHECK (is_bool (tree ("true")));
  CHECK (is_bool (tree ("false")));
  CHECK (!is_bool (tree ("other")));
}

TEST_CASE ("test_is_int") {
  CHECK (!is_int (tree ()));
  CHECK (is_int (tree ("+12")));
  CHECK (is_int (tree ("-12")));
  CHECK (!is_int (tree ("-+12")));
  CHECK (!is_int (tree ("one")));
}

TEST_CASE ("test_is_func") {
  CHECK (!is_func (tree (70, 3), 5));
  CHECK (!is_func (tree (), 0));
  CHECK (is_func (tree (67, 4), 67));
}

TEST_CASE ("test_is_double") {
  CHECK (!is_double (tree ()));
  CHECK (is_double (tree ("3.15")));
  CHECK (is_double (tree ("0")));
  CHECK (is_double (tree ("0.0")));
  CHECK (is_double (tree ("3.1415926")));
  CHECK (is_double (tree ("-3.1415926")));
  CHECK (!is_double (tree ("nothing")));
}

TEST_CASE ("test_is_string") {
  CHECK (is_string (tree ()));
  CHECK (is_string (tree ("0")));
  CHECK (is_string (tree ("+121234")));
  CHECK (is_string (tree ("seigj")));
  CHECK (is_string (tree ("!@#$$!")));
  CHECK (!is_string (tree (70, 3)));
}

TEST_CASE ("test_is_generic") {
  CHECK (!is_generic (tree (9, 0)));
  CHECK (!is_generic (tree (10, -1)));
  CHECK (!is_generic (tree (11, 1234)));
  CHECK (!is_generic (tree (11, 4)));
  CHECK (!is_generic (tree (12, 2)));
  CHECK (is_generic (tree (-1, 1)));
  CHECK (is_generic (tree (-2, 1)));
  CHECK (is_generic (tree (-3, 1)));
}

TEST_CASE ("test_arity") {
  CHECK (!arity (tree (0, -1)));
  CHECK (!arity (tree (280, 0)));
  CHECK (arity (tree (9, 3)));
  CHECK (!arity (tree (0, 5)));
}
