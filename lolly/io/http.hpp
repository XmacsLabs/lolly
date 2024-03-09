
/******************************************************************************
 * MODULE     : http.hpp
 * DESCRIPTION: HTTP related routines
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "blackbox.hpp"
#include "hashmap.hpp"
#include "lolly/data/lolly_tree.hpp"
#include "url.hpp"

namespace lolly {
namespace io {

typedef lolly_tree<blackbox> http_tree;

enum http_label : int {
  STATUS_CODE= 1,
  TEXT,
  URL,
  ELAPSED,
  HEADER,
  PARAMETERS,
  PAYLOAD,
  MULTIPART,
  ROOT,
  TUPLE,
};

using http_headers= hashmap<string, string>;

inline http_tree
http_response_init () {
  http_tree ret        = http_tree (http_label::ROOT, 0);
  http_tree status_code= http_tree (http_label::STATUS_CODE);
  http_tree elapsed    = http_tree (http_label::ELAPSED);
  http_tree header     = http_tree (http_label::HEADER);

  elapsed->data    = close_box<double> (0.0);
  status_code->data= close_box<long> (404);
  auto hmap        = hashmap<string, string> ();
  header->data     = close_box (hmap);

  ret << status_code;
  ret << http_tree (http_label::TEXT, http_tree (""));
  ret << http_tree (http_label::URL, http_tree (""));
  ret << elapsed;
  ret << header;
  return ret;
}

inline http_tree
http_response_ref (http_tree r, http_label op) {
  return r[op - 1][0];
}

inline void
http_response_set (http_tree r, http_label op, http_tree t) {
  r[op - 1][0]= t;
}

http_tree http_get (url u, http_headers headers= http_headers ());
http_tree http_head (url u, http_headers headers= http_headers ());
http_tree download (url from, url to, http_headers headers= http_headers ());

} // namespace io
} // namespace lolly
