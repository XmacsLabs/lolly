
/******************************************************************************
 * MODULE     : subprocess_test.cpp
 * DESCRIPTION: tests on subprocess related routines
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "a_lolly_test.hpp"
#include "analyze.hpp"
#include "lolly/system/subprocess.hpp"
#include "sys_utils.hpp"

using lolly::system::call;
using lolly::system::check_output;
using lolly::system::parse_command_line;

TEST_MEMORY_LEAK_INIT

TEST_CASE ("check_output") {
  string stdout_result;
  string stderr_result;
  if (!os_wasm () && !os_mingw ()) {
    lolly::system::check_stdout ("xmake --version", stdout_result);
    CHECK (N (stdout_result) > 0);
    lolly::system::check_stderr ("ls /no_such_dir", stderr_result);
    CHECK (N (stderr_result) > 0);
  }
}

TEST_CASE ("call") {
#ifdef OS_WASM
  CHECK_EQ (call ("xmake --version"), -1);
  CHECK_EQ (call ("no_such_command"), -1);
  CHECK_EQ (call (""), -1);
#else
#ifndef OS_MINGW
  CHECK (call ("xmake --version") == 0);
#endif
  CHECK (call ("no_such_command") != 0);
  CHECK (call ("") != 0);
#endif
}

TEST_CASE ("parse_command_line") {
  array<string> arr;
  arr << "python3"
      << "-c" << raw_quote ("print(\\\"hello\\n\\\")");
  string cmd= recompose (arr, " ");
  string_eq (cmd, "python3 -c \"print(\\\"hello\\n\\\")\"");
  CHECK (parse_command_line (cmd) == arr);
}

TEST_MEMORY_LEAK_ALL
