/** \file http_test.cpp
 *  \copyright GPLv3
 *  \details Unitests for http.
 *  \author Darcy Shen
 *  \date   2023
 */

#include "a_tbox_main.cpp"
#include "blackbox.hpp"
#include "generic_tree.hpp"
#include "lolly/io/http.hpp"

TEST_CASE ("http::get") {
#ifndef OS_WASM
  tree r= lolly::io::http_get ("https://httpbin.org/get");
  CHECK_EQ (as<tree, long> (r[0][0]), 200);
#endif
}
