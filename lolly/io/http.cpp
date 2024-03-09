
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

const char* HTTP_USER_AGENT= "User-Agent";

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
  http_tree status_code= http_tree (http_label::STATUS_CODE);
  status_code->data    = close_box<long> (r.status_code);

  http_tree elapsed= http_tree (http_label::ELAPSED);
  elapsed->data    = close_box<double> (r.elapsed);

  auto hmap= hashmap<string, string> ();
  for (auto i= r.header.begin (); i != r.header.end (); i++) {
    string key  = locase_all (string (i->first.c_str ()));
    string value= string (i->second.c_str ());
    hmap (key)  = value;
  }
  http_tree header= http_tree (http_label::HEADER);
  header->data    = close_box (hmap);

  http_tree ret= http_response_init ();
  http_response_set (ret, STATUS_CODE, status_code);
  http_response_set (ret, TEXT, http_tree (r.text.c_str ()));
  http_response_set (ret, URL, http_tree (url));
  http_response_set (ret, ELAPSED, elapsed);
  http_response_set (ret, HEADER, header);
  return ret;
}

static cpr::Header
as_cpr_header (http_headers hmap) {
  cpr::Header      header= cpr::Header{};
  iterator<string> it    = iterate (hmap);
  while (it->busy ()) {
    string key                          = it->next ();
    header[std::string (c_string (key))]= c_string (hmap[key]);
  }
  return header;
}

http_tree
http_get (url u, http_headers headers) {
  string       u_str = as_string (u);
  c_string     u_cstr= c_string (u_str);
  cpr::Session session;
  session.SetUrl (cpr::Url (u_cstr));
  session.SetHeader (as_cpr_header (headers));
  session.SetUserAgent (
      cpr::UserAgent (std::string (c_string (headers[HTTP_USER_AGENT]))));
  cpr::Response r= session.Get ();
  return response_to_tree (r, u_str);
}

http_tree
http_head (url u, http_headers headers) {
  string       u_str = as_string (u);
  c_string     u_cstr= c_string (u_str);
  cpr::Session session;
  session.SetUrl (cpr::Url (u_cstr));
  session.SetHeader (as_cpr_header (headers));
  session.SetUserAgent (
      cpr::UserAgent (std::string (c_string (headers[HTTP_USER_AGENT]))));
  cpr::Response r= session.Head ();
  return response_to_tree (r, u_str);
}

http_tree
download (url from, url to, http_headers headers) {
  string   from_str = as_string (from);
  c_string from_cstr= c_string (from_str);
  string   to_str   = as_string (to);
  c_string to_cstr  = c_string (to_str);

  cpr::Session session;
  session.SetUrl (cpr::Url (from_cstr));
  session.SetHeader (as_cpr_header (headers));
  session.SetUserAgent (
      cpr::UserAgent (std::string (c_string (headers[HTTP_USER_AGENT]))));
  std::ofstream to_stream (to_cstr, std::ios::binary);
  cpr::Response r= session.Download (to_stream);
  return response_to_tree (r, from_str);
}

#endif

} // namespace io
} // namespace lolly
