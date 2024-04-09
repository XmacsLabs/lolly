
/******************************************************************************
 * MODULE     : shared_lib_test.cpp
 * DESCRIPTION: tests on dynamic library loading
 * COPYRIGHT  : (C) 2024  jingkaimori
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "a_lolly_test.hpp"
#include "lolly/system/shared_lib.hpp"
#include "sys_utils.hpp"

using lolly::system::load_shared_library;
using lolly::system::shared_lib;

TEST_MEMORY_LEAK_INIT

TEST_CASE ("check_output") {
  shared_lib lib          = load_shared_library ("test", url_none ());
  void       (*func) (int)= lib->get_function<void, int> ("func");
  func (0);
}

TEST_MEMORY_LEAK_ALL
