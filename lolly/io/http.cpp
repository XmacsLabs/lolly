
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
#include "lolly/data/uri.hpp"
#include "tree.hpp"

#include <cpr/cpr.h>

namespace lolly {
namespace io {

json
http_get (url u) {
  c_string      u_cstr= c_string (as_string (u));
  cpr::Response r     = cpr::Get (cpr::Url{u_cstr});
  json          r_json;
  r_json.set ("status_code", r.status_code);
  r_json.set ("url", r.url.c_str ());
  r_json.set ("text", r.text.c_str ());
  json headers_json;
  for (auto i= r.header.begin (); i != r.header.end (); i++) {
    headers_json.set (string (i->first.c_str ()), string (i->second.c_str ()));
  }
  r_json.set ("headers", headers_json);
  return r_json;
}

} // namespace io
} // namespace lolly
