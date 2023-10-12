/** \file http_test.cpp
 *  \copyright GPLv3
 *  \details Unitests for http.
 *  \author Darcy Shen
 *  \date   2023
 */

#include "a_tbox_main.cpp"
#include "lolly/io/http.hpp"

TEST_CASE ("http::get") {
#ifndef OS_WASM
  json j= lolly::io::http_get ("https://httpbin.org/get");
  CHECK (N (as_string (j ("text"))) > 0);
#endif
}
