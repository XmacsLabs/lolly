
#include "a_lolly_test.hpp"
#include "analyze.hpp"
#include "json.hpp"
#include "string.hpp"
#include "tm_ostream.hpp"

using json= lolly::data::json;

TEST_CASE ("as_string") {
  string_eq (json ().to_string (), "{}");
  string_eq (json ("string").to_string (), raw_quote ("string"));
  string_eq (json (true).to_string (), raw_quote ("true"));
  string_eq (json (false).to_string (), raw_quote ("false"));
  string_eq (json (1).to_string (), "1");
  string_eq (json (2.11).to_string (), "2.11");
  string_eq (json::json_null ().to_string (), raw_quote ("null"));
}

TEST_CASE ("access") {
  json j= json ();
  j.set ("name", "Bob");
  string_eq (as_string (j ("name")), "Bob");
  j.set ("name", "John");
  string_eq (as_string (j ("name")), "John");
  j.set ("age", 12);
  string_eq (j.to_string (), "{\"name\":\"John\",\"name\":12}");
}
