#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#undef FAILED
#undef ERROR

#include "tree.hpp"
using namespace lolly;

TEST_CASE ("test is atomic") { CHECK (is_atomic (tree ())); }

TEST_CASE ("test is tuple") {
  CHECK (is_tuple (tuple ()));
  CHECK (is_tuple (tuple (tree ())));
  CHECK (is_tuple (tuple (tree (), tree ())));
  CHECK (is_tuple (tuple (tree (), tree (), tree ())));
  CHECK (is_tuple (tuple (tree (), tree (), tree (), tree ())));
  CHECK (is_tuple (tuple (tree (), tree (), tree (), tree (), tree ())));
}

TEST_CASE ("test is concat") {
  CHECK (is_concat (concat ()));
  CHECK (is_concat (concat (tree ())));
  CHECK (is_concat (concat (tree (), tree ())));
  CHECK (is_concat (concat (tree (), tree (), tree ())));
  CHECK (is_concat (concat (tree (), tree (), tree (), tree ())));
  CHECK (is_concat (concat (tree (), tree (), tree (), tree (), tree ())));
}

TEST_CASE ("test is compound") {
  CHECK (is_compound (tree (FRAC, 3)));
  CHECK (!is_compound (tree ()));
}

TEST_CASE ("test is bool") {
  CHECK (is_bool (tree ("true")));
  CHECK (is_bool (tree ("false")));
  CHECK (!is_bool (tree ("other")));
}

TEST_CASE ("test is int") {
  CHECK (!is_int (tree ()));
  CHECK (is_int (tree ("+12")));
  CHECK (is_int (tree ("-12")));
  CHECK (!is_int (tree ("-+12")));
  CHECK (!is_int (tree ("one")));
}

TEST_CASE ("test is func") {
  CHECK (!is_func (tree (FRAC, 3), tree_label (DOCUMENT)));
  CHECK (!is_func (tree (), tree_label ()));
  CHECK (is_func (tree (LSUP, 4), tree_label (LSUP)));
}

TEST_CASE ("test is double") {
  CHECK (!is_double (tree ()));
  CHECK (is_double (tree ("3.15")));
  CHECK (is_double (tree ("0")));
  CHECK (is_double (tree ("0.0")));
  CHECK (is_double (tree ("3.1415926")));
  CHECK (is_double (tree ("-3.1415926")));
  CHECK (!is_double (tree ("nothing")));
}

TEST_CASE ("test is string") {
  CHECK (is_string (tree ()));
  CHECK (is_string (tree ("0")));
  CHECK (is_string (tree ("+121234")));
  CHECK (is_string (tree ("seigj")));
  CHECK (is_string (tree ("!@#$$!")));
  CHECK (!is_string (tree (FRAC, 3)));
}

TEST_CASE ("test is generic") {
  CHECK (!is_generic (tree (RIGID, 0)));
  CHECK (!is_generic (tree (PARA, -1)));
  CHECK (!is_generic (tree (RIGID, 1234)));
  CHECK (!is_generic (tree (IF_PAGE_BREAK, 4)));
  CHECK (!is_generic (tree (RIGID, 2)));
}

TEST_CASE ("test get label") {
  CHECK (get_label (tree ("1")) != string ("_POINT"));
  CHECK (get_label (tree (ARC, 0)) != string ("RIGID"));
  CHECK (get_label (tree (ARC, -1)) != string ("CARC"));
}

TEST_CASE ("test arity") {
  CHECK (!arity (tree (STRING, -1)));
  CHECK (!arity (tree (ARC, 0)));
  CHECK (arity (tree (RIGID, 3)));
  CHECK (!arity (tree (STRING, 5)));
}
