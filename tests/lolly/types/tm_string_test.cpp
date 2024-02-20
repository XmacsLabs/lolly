#include "doctest/doctest.h"
#include "lolly/data/tm_string.hpp"

using namespace lolly::data;

TEST_CASE ("equality of string") {
  CHECK_EQ (stringU16 (u"abc") == u"abc", true);
  CHECK_EQ (stringU16 (u"abc") == u"", false);

  CHECK_EQ (stringU16 (u"abc") != u"abc", false);
  CHECK_EQ (stringU16 (u"abc") != u"", true);

  CHECK_EQ (stringU16 (u"abc") == stringU16 (u"abc"), true);
  CHECK_EQ (stringU16 (u"abc") == stringU16 (), false);
  CHECK_EQ (stringU16 (u"abc") != stringU16 (u"abc"), false);
  CHECK_EQ (stringU16 (u"abc") != stringU16 (), true);

  CHECK_EQ (stringU16 () == stringU16 (), true);
}

TEST_CASE ("compare string") {
  CHECK (stringU16 (u"ab") < stringU16 (u"b"));
  CHECK (!(stringU16 (u"b") < stringU16 (u"ab")));
  CHECK (stringU16 () < stringU16 (u"0"));
  CHECK (!(stringU16 (u"0") < stringU16 ()));
  CHECK (stringU16 (u"a") <= stringU16 (u"a"));
  CHECK (!(stringU16 (u"ab") <= stringU16 (u"a")));
  CHECK (stringU16 (u"ab") <= stringU16 (u"b"));
  CHECK (!(stringU16 (u"b") <= stringU16 (u"ab")));
  CHECK (stringU16 () <= stringU16 ());
  CHECK (stringU16 () <= stringU16 (u"0"));
  CHECK (!(stringU16 (u"0") <= stringU16 ()));
}

TEST_CASE ("test slice") {
  CHECK_EQ (stringU16 (u"abcde") (0, 0) == stringU16 (), true);
  CHECK_EQ (stringU16 (u"abcde") (0, 1) == stringU16 (u"a"), true);
  CHECK_EQ (stringU16 (u"abcde") (1, 3) (0, 1) == stringU16 (u"b"), true);
  CHECK_EQ (stringU16 (u"abcde") (0, 10) == stringU16 (u"abcde"), true);
  CHECK_EQ (stringU16 (u"abcde") (-1, 1) == stringU16 (u"a"), true);
  CHECK_EQ (stringU16 (u"abcde") (3, 2) == stringU16 (), true);
  CHECK_EQ (stringU16 (u"abcde") (3, -2) == stringU16 (), true);
  CHECK_EQ (stringU16 (u"abcde") (10, 11) == stringU16 (), true);
  CHECK_EQ (stringU16 (u"abcde") (-3, -2) == stringU16 (), true);
}

TEST_CASE ("test concat") {
  CHECK_EQ (stringU16 (u"abc") * u"de" == stringU16 (u"abcde"), true);
  CHECK_EQ (stringU16 (u"abc") * stringU16 (u"de") == stringU16 (u"abcde"),
            true);
  CHECK_EQ (u"abc" * stringU16 (u"de") == stringU16 (u"abcde"), true);
}

/******************************************************************************
 * Modifications
 ******************************************************************************/

TEST_CASE ("test append") {
  auto str= stringU16 ();
  str << u'x';
  CHECK_EQ (str == stringU16 (u"x"), true);
  str << stringU16 (u"yz");
  CHECK_EQ (str == stringU16 (u"xyz"), true);
}
