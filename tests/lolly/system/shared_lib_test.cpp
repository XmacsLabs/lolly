
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

#ifndef OS_WASM
using lolly::system::load_shared_library;
using lolly::system::shared_lib;
#endif

TEST_CASE ("check_output") {
#ifndef OS_WASM
  shared_lib lib      = load_shared_library ("example_dynamic_library",
                                             url_pwd () * "example_dynamic_library");
  double (*func) (int)= lib->get_function<double, int> ("pow_div_2");
  double res          = func (5);
  CHECK_EQ (res, 12.5);
#endif
}
