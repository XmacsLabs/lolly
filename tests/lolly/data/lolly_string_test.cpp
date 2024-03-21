/** \file lolly_string_test.cpp
 *  \copyright GPLv3
 *  \details Test for string
 *  \author jingkaimori
 *  \date   2024
 */

#include "doctest/doctest.h"
#include "lolly/data/lolly_string.hpp"

using namespace lolly::data;

using string_u16_view= lolly::data::lolly_string_view<char16_t>;

TEST_CASE ("construct string_view") {
  CHECK (string_u16_view (u"abc") == u"abc");

  const char16_t* str= u"def";
  CHECK (string_u16_view (3, str) == u"def");

  const std::u16string std_str (u"ghij");
  CHECK (string_u16_view (std_str) == u"ghij");
}

TEST_CASE ("equality of string") {
  CHECK_EQ (string_u16 (u"abc") == u"abc", true);
  CHECK_EQ (string_u16 (u"abc") == u"", false);

  CHECK_EQ (string_u16 (u"abc") != u"abc", false);
  CHECK_EQ (string_u16 (u"abc") != u"", true);

  CHECK_EQ (string_u16 (u"abc") == string_u16 (u"abc"), true);
  CHECK_EQ (string_u16 (u"abc") == string_u16 (), false);
  CHECK_EQ (string_u16 (u"abc") != string_u16 (u"abc"), false);
  CHECK_EQ (string_u16 (u"abc") != string_u16 (), true);

  CHECK_EQ (string_u16 () == string_u16 (), true);
}

TEST_CASE ("compare string") {
  CHECK (string_u16 (u"ab") < string_u16 (u"b"));
  CHECK (!(string_u16 (u"b") < string_u16 (u"ab")));
  CHECK (string_u16 () < string_u16 (u"0"));
  CHECK (!(string_u16 (u"0") < string_u16 ()));
  CHECK (string_u16 (u"a") <= string_u16 (u"a"));
  CHECK (!(string_u16 (u"ab") <= string_u16 (u"a")));
  CHECK (string_u16 (u"ab") <= string_u16 (u"b"));
  CHECK (!(string_u16 (u"b") <= string_u16 (u"ab")));
  CHECK (string_u16 () <= string_u16 ());
  CHECK (string_u16 () <= string_u16 (u"0"));
  CHECK (!(string_u16 (u"0") <= string_u16 ()));
}

TEST_CASE ("test slice") {
  CHECK_EQ (string_u16 (u"abcde") (0, 0) == string_u16 (), true);
  CHECK_EQ (string_u16 (u"abcde") (0, 1) == string_u16 (u"a"), true);
  CHECK_EQ (string_u16 (u"abcde") (1, 3) (0, 1) == string_u16 (u"b"), true);
  CHECK_EQ (string_u16 (u"abcde") (0, 10) == string_u16 (u"abcde"), true);
  CHECK_EQ (string_u16 (u"abcde") (-1, 1) == string_u16 (u"a"), true);
  CHECK_EQ (string_u16 (u"abcde") (3, 2) == string_u16 (), true);
  CHECK_EQ (string_u16 (u"abcde") (3, -2) == string_u16 (), true);
  CHECK_EQ (string_u16 (u"abcde") (10, 11) == string_u16 (), true);
  CHECK_EQ (string_u16 (u"abcde") (-3, -2) == string_u16 (), true);
}

TEST_CASE ("test concat") {
  CHECK_EQ (string_u16 (u"abc") * u"de" == string_u16 (u"abcde"), true);
  CHECK_EQ (string_u16 (u"abc") * string_u16 (u"de") == string_u16 (u"abcde"),
            true);
  CHECK_EQ (u"abc" * string_u16 (u"de") == string_u16 (u"abcde"), true);
}

/******************************************************************************
 * Modifications
 ******************************************************************************/

TEST_CASE ("test append") {
  auto str= string_u16 ();
  str << u'x';
  CHECK_EQ (str == string_u16 (u"x"), true);
  str << string_u16 (u"yz");
  CHECK_EQ (str == string_u16 (u"xyz"), true);
}

TEST_CASE ("test reserve along with append") {

  SUBCASE ("reserved more space") {
    auto str= string_u16 ();
    str->reserve (6);
    str << u'x';
    CHECK_EQ (str == u"x", true);
    str << string_u16 (u"yz");
    CHECK_EQ (str == u"xyz", true);
    str << string_u16 (u": larger than reserved space");
    CHECK_EQ (str == u"xyz: larger than reserved space", true);
  }
  SUBCASE ("reserved the same space") {
    auto str= string_u16 (u"abc");
    str->reserve (3);
    CHECK_EQ (str == u"abc", true);
  }
  SUBCASE ("reserved less space should take no effect") {
    auto str= string_u16 (u"abc");
    str->reserve (2);
    CHECK_EQ (str == u"abc", true);
  }
}

TEST_CASE ("test expand_or_shrink_by along with sub index") {

  SUBCASE ("expand") {
    auto str       = string_u16 (u"abc");
    int  previous_n= str->expand_or_shrink_by (1);
    CHECK_EQ (previous_n, 3);
    str[3]= u'd';
    CHECK_EQ (str == u"abcd", true);
  }
  SUBCASE ("shrink") {
    auto str       = string_u16 (u"abc");
    int  previous_n= str->expand_or_shrink_by (-1);
    CHECK_EQ (previous_n, 3);
    CHECK_EQ (str == u"ab", true);
  }
  SUBCASE ("delta 0 takes no effect") {
    auto str       = string_u16 (u"abc");
    int  previous_n= str->expand_or_shrink_by (0);
    CHECK_EQ (previous_n, 3);
    CHECK_EQ (str == u"abc", true);
  }
}

TEST_CASE ("test resize") {

  SUBCASE ("expand") {
    auto str= string_u16 (u"abc");
    str->resize (4);
    str[3]= u'd';
    CHECK_EQ (str == u"abcd", true);
  }
  SUBCASE ("shrink") {
    auto str= string_u16 (u"abc");
    str->resize (2);
    CHECK_EQ (str == u"ab", true);
  }
  SUBCASE ("delta 0 takes no effect") {
    auto str= string_u16 (u"abc");
    str->resize (3);
    CHECK_EQ (str == u"abc", true);
  }
}
