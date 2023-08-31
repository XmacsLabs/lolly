#include "url.hpp"

file_url::file_url (string name)
    : url (url_root ("file") * url_get_name (name)) {}

file_url::file_url (const char* name)
    : url (url_root ("file") * url_get_name (string (name))) {}

string
file_url::concretize () {
  url c= reroot (*this, "default");
  if (!is_none (c)) return as_string (c);
  if (is_wildcard (*this, 1)) return (*this)->t[1]->label;
  TM_FAILED ("failed to concretize file_url");
}
