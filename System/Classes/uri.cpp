#include "uri.hpp"

string
uri_host (url u) {
  return as_string (u[2][1]);
}

string
uri_path (url u) {
  return string ("/") * as_string (u[2][1], URL_STANDARD);
}
