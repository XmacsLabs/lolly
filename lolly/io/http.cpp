
/******************************************************************************
 * MODULE     : http.cpp
 * DESCRIPTION: HTTP related routines
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "http.hpp"
#include "generic_tree.hpp"
#include "http_response.hpp"
#include "lolly/data/uri.hpp"
#include "tree.hpp"

#ifndef OS_WASM
#include <cpr/cpr.h>
#endif

namespace lolly {
namespace io {

tree
http_response_ref (tree response, http_response_label op) {
  return response[op - 1][0];
}

#ifdef OS_WASM
tree
http_get (url u) {
  return tree ();
}
#else
tree
http_get (url u) {
  string        u_str = as_string (u);
  c_string      u_cstr= c_string (u_str);
  cpr::Response r     = cpr::Get (cpr::Url{u_cstr});
  tree          ret   = tree (http_response_label::ROOT, 0);
  ret << tree (http_response_label::STATUS_CODE,
               as<long, tree> (r.status_code));
  ret << tree (http_response_label::TEXT, tree (r.text.c_str ()));
  ret << tree (http_response_label::URL, tree (u_str));
  ret << tree (http_response_label::ELAPSED, as<double, tree> (r.elapsed));
  auto hmap= hashmap<string, string> ();
  for (auto i= r.header.begin (); i != r.header.end (); i++) {
    hmap (string (i->first.c_str ()))= string (i->second.c_str ());
  }
  tree header= tree (http_response_label::HEADER,
                     as<hashmap<string, string>, tree> (hmap));
  ret << header;

  return ret;
}
#endif

} // namespace io
} // namespace lolly
