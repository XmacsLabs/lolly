
/******************************************************************************
 * MODULE     : http_request.hpp
 * DESCRIPTION: Tree repr of HTTP Request
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "generic_tree.hpp"
#include "hashmap.hpp"
#include "tree.hpp"

namespace lolly {
namespace io {
enum http_request_label : int {
  HEADERS= 1,
  PARAMETERS,
  PAYLOAD,
  MULTIPART,
};

inline tree
http_headers_init () {
  auto hmap   = hashmap<string, string> ();
  tree headers= tree (http_request_label::HEADERS,
                      as<hashmap<string, string>, tree> (hmap));
  return headers;
}

} // namespace io
} // namespace lolly
