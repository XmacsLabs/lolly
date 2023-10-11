#include "http.hpp"
#include "tree.hpp"
#include "uri.hpp"

#include <httplib.h>

namespace lolly {
namespace http {

json
get (url u) {
  string          host= uri_host (u);
  string          path= uri_path (u);
  httplib::Client cli ((char*) c_string (host));
  auto            r= cli.Get ((char*) c_string (path));
  json            r_json;
  r_json.set ("status", r->status);
  r_json.set ("body", (r->body).c_str ());
  return r_json;
}

} // namespace http
} // namespace lolly
