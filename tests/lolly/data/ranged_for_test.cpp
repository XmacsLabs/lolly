
/******************************************************************************
 * MODULE     : ranged_for_test.cpp
 * DESCRIPTION: tests on ranged for clause with lightweight iterator
 * COPYRIGHT  : (C) 2024  jingkaimori
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "a_lolly_test.hpp"
#include "lolly/data/ranged_for.hpp"
#include "string.hpp"

using namespace lolly::data;

TEST_CASE ("on string") {
  string s ("character");
  string res;
  for (auto&& ch : iterator_lite (s)) {
    res << ch + ('A' - 'a');
  }
  string_eq (res, "CHARACTER");
}

TEST_CASE ("iterable types") {
  CHECK (iterable_with_subscript<string>::value);
  CHECK_FALSE (iterable_with_subscript<string_view<char>>::value);
  CHECK_FALSE (iterable_with_subscript<c_string>::value);
}
