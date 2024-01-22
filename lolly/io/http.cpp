
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
#include "generic_tree.hpp"
#include "hashmap.hpp"
#include "lolly/data/uri.hpp"
#include "lolly/io/http_response.hpp"
#include "tree.hpp"

#ifndef OS_WASM
#include <cpr/cpr.h>
#endif

namespace lolly {
namespace io {

#ifdef OS_WASM

tree
http_head (url u, tree headers) {
  return http_response_init ();
}

tree
http_get (url u, tree headers) {
  return http_response_init ();
}

tree
download (url from, url to, tree headers) {
  return http_response_init ();
}

#else

static tree
response_to_tree (cpr::Response r, string url) {
  tree ret= http_response_init ();
  http_response_set (ret, STATUS_CODE, as<long, tree> (r.status_code));
  http_response_set (ret, TEXT, tree (r.text.c_str ()));
  http_response_set (ret, URL, tree (url));
  http_response_set (ret, ELAPSED, as<double, tree> (r.elapsed));
  auto hmap= hashmap<string, string> ();
  for (auto i= r.header.begin (); i != r.header.end (); i++) {
    string key  = locase_all (string (i->first.c_str ()));
    string value= string (i->second.c_str ());
    hmap (key)  = value;
  }
  http_response_set (ret, HEADER, as<hashmap<string, string>, tree> (hmap));
  return ret;
}

static cpr::Header
hashmap_to_header (hashmap<string, string> hmap) {
  cpr::Header      header= cpr::Header{};
  iterator<string> it    = iterate (hmap);
  while (it->busy ()) {
    string key                          = it->next ();
    header[std::string (c_string (key))]= c_string (hmap[key]);
  }
  return header;
}

tree
http_get (url u, hashmap<string, string> headers) {
  string       u_str = as_string (u);
  c_string     u_cstr= c_string (u_str);
  cpr::Session session;
  session.SetUrl (cpr::Url (u_cstr));
  session.SetHeader (hashmap_to_header (headers));
  session.SetUserAgent (
      cpr::UserAgent (std::string (c_string (headers["User-Agent"]))));
  cpr::Response r= session.Get ();
  return response_to_tree (r, u_str);
}

tree
http_head (url u, hashmap<string, string> headers) {
  string       u_str = as_string (u);
  c_string     u_cstr= c_string (u_str);
  cpr::Session session;
  session.SetUrl (cpr::Url (u_cstr));
  session.SetHeader (hashmap_to_header (headers));
  session.SetUserAgent (
      cpr::UserAgent (std::string (c_string (headers["User-Agent"]))));
  cpr::Response r= session.Head ();
  return response_to_tree (r, u_str);
}

tree
download (url from, url to, hashmap<string, string> headers) {
  string   from_str = as_string (from);
  c_string from_cstr= c_string (from_str);
  string   to_str   = as_string (to);
  c_string to_cstr  = c_string (to_str);

  cpr::Session session;
  session.SetUrl (cpr::Url (from_cstr));
  session.SetHeader (hashmap_to_header (headers));
  session.SetUserAgent (
      cpr::UserAgent (std::string (c_string (headers["User-Agent"]))));
  std::ofstream to_stream (to_cstr, std::ios::binary);
  cpr::Response r= session.Download (to_stream);
  return response_to_tree (r, from_str);
}

#endif

} // namespace io
} // namespace lolly
