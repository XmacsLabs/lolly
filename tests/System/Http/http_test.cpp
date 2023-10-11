/** \file file.cpp
 *  \copyright GPLv3
 *  \details Unitests for file.
 *  \author Darcy
 *  \date   2019-2023
 */

#include "a_tbox_main.cpp"
#include "http.hpp"

TEST_CASE ("http::get") {
  json   j   = lolly::http::get ("https://httpbin.org/get");
  string body= j["body"].dump ().c_str ();
  CHECK (N (body) > 0);
}
