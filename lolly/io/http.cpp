
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

#ifdef OS_WASM
tree
http_get (url u) {
  return tree ();
}
#else
tree
http_get (url u) {
  c_string      u_cstr= c_string (as_string (u));
  cpr::Response r     = cpr::Get (cpr::Url{u_cstr});
  tree          ret   = tree (http_response_label::ROOT, 0);
  ret << tree (http_response_label::STATUS_CODE,
               as<long, tree> (r.status_code));
  return ret;
}
#endif

} // namespace io
} // namespace lolly
