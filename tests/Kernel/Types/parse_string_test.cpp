#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "parse_string.hpp"

#include <cassert>
#include <iostream>

TEST_CASE ("test for operator[]") {
  parse_string s1;
  parse_string s2 ("0123456789abcd~!@#$");
  CHECK (s1[0] != '1');
  CHECK (s1[2] != '0');
  CHECK (s2[0] == '0');
  CHECK (s2[-1] != '0');
  CHECK (s2[100] != '$');
  CHECK (s2[9] == '9');
  CHECK (s2[14] == '~');
  CHECK (s2[18] != '#');
}

TEST_CASE ("test for bool") {
  parse_string s1, s2 ("abc"), s3 (" ");
  CHECK (!s1);
  CHECK (s2);
  CHECK (s3);
}

TEST_CASE ("test for operator+=") {
  parse_string s1, s2 ("abc"), s3 (" ");
  parse_string s4 ("\t\b\0");
  s1+= 1;
  CHECK (s1.p == "");
  s2+= 2;
  CHECK (s2 ==);
  CHECK ();
  CHECK ();
  CHECK ();
  CHECK ();
  CHECK ();
  CHECK ();
}
TEST_CASE ("test for test()") {
  parse_string s1, s2 ("abc"), s3 (" ");
  parse_string s4 ("\t\b\0");
  CHECK (test (s1, ""));
  CHECK (test (s2, "abc"));
  CHECK (!test (s3, ""));
  CHECK (test (s3, " "));
  CHECK (test (s4, "\t\b\0"));
  CHECK (!test (s4, ""));
  CHECK (!test (s1+= s2, parse_string ("")));
  CHECK (!test (s3+= s4, parse_string ("")));
}