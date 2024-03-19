
/******************************************************************************
 * MODULE     : http.cpp
 * DESCRIPTION: HTTP related routines
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "lolly/io/http.hpp"
#include "analyze.hpp"
#include "hashmap.hpp"
#include "iterator.hpp"
#include "lolly/data/uri.hpp"

#ifndef OS_WASM
#include <cpr/cpr.h>
#endif

namespace lolly {
namespace io {

const string HTTP_USER_AGENT ("User-Agent");

template <typename T>
inline http_tree
blackbox_tree (int label, T data) {
  http_tree ret= http_tree (label);
  ret->data    = close_box<T> (data);
  return ret;
}

#ifdef OS_WASM

http_tree
http_head (url u, http_headers headers) {
  return http_response_init ();
}

http_tree
http_get (url u, http_headers headers) {
  return http_response_init ();
}

http_tree
download (url from, url to, http_headers headers) {
  return http_response_init ();
}

#else

static http_tree
response_to_tree (cpr::Response r, string url) {
  http_tree ret= http_response_init ();
  http_response_set (
      ret, STATUS_CODE,
      blackbox_tree<long> (http_label::STATUS_CODE, r.status_code));
  http_response_set (
      ret, TEXT,
      blackbox_tree<string> (http_label::TEXT, as_string (r.text.c_str ())));
  http_response_set (ret, URL,
                     blackbox_tree<string> (http_label::URL, string (url)));
  http_response_set (ret, ELAPSED,
                     blackbox_tree<double> (http_label::ELAPSED, r.elapsed));

  auto hmap= hashmap<string, string> ();
  for (auto i= r.header.begin (); i != r.header.end (); i++) {
    string key  = locase_all (as_string (i->first.c_str ()));
    string value= as_string (i->second.c_str ());
    hmap (key)  = value;
  }
  http_tree header=
      blackbox_tree<hashmap<string, string>> (http_label::HEADER, hmap);
  http_response_set (ret, HEADER, header);
  return ret;
}

using string_view= lolly::data::lolly_string_view<char>;

static cpr::Header
as_cpr_header (http_headers hmap) {
  cpr::Header      header= cpr::Header{};
  iterator<string> it    = iterate (hmap);
  while (it->busy ()) {
    string key               = it->next ();
    header[(string_view) key]= (string_view) hmap[key];
  }
  return header;
}

http_tree
http_get (url u, http_headers headers) {
  string       u_str= as_string (u);
  cpr::Session session;
  session.SetUrl (cpr::Url ((string_view) u_str));
  session.SetHeader (as_cpr_header (headers));
  session.SetUserAgent (
      cpr::UserAgent ((string_view) headers[HTTP_USER_AGENT]));
  cpr::Response r= session.Get ();
  return response_to_tree (r, u_str);
}

http_tree
http_head (url u, http_headers headers) {
  string       u_str= as_string (u);
  cpr::Session session;
  session.SetUrl (cpr::Url ((string_view) u_str));
  session.SetHeader (as_cpr_header (headers));
  session.SetUserAgent (
      cpr::UserAgent ((string_view) headers[HTTP_USER_AGENT]));
  cpr::Response r= session.Head ();
  return response_to_tree (r, u_str);
}

http_tree
download (url from, url to, http_headers headers) {
  string from_str= as_string (from);
  string to_str  = as_string (to);

  cpr::Session session;
  session.SetUrl (cpr::Url ((string_view) from_str));
  session.SetHeader (as_cpr_header (headers));
  session.SetUserAgent (
      cpr::UserAgent ((string_view) headers[HTTP_USER_AGENT]));
  std::ofstream to_stream ((string_view) to_str, std::ios::binary);
  cpr::Response r= session.Download (to_stream);
  return response_to_tree (r, from_str);
}

#endif

} // namespace io
} // namespace lolly
