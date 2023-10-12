/** \file http_test.cpp
 *  \copyright GPLv3
 *  \details Unitests for http.
 *  \author Darcy Shen
 *  \date   2023
 */

#include "a_tbox_main.cpp"
#include "http.hpp"

TEST_CASE ("http::get") {
  json   j   = lolly::http::get ("https://httpbin.org/get");
  string body= j ("body").dump ();
  CHECK (N (body) > 0);
}
