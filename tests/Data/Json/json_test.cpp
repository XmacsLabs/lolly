
#include "a_lolly_test.hpp"
#include "analyze.hpp"
#include "json.hpp"
#include "string.hpp"
#include "tm_ostream.hpp"

using lson= lolly::data::json;

TEST_CASE ("as_string") {
  lson l1= lson ();
  string_eq (l1.to_string (), "{}");
  lson l2= lson (string ("string"));
  string_eq (l2.to_string (), raw_quote ("string"));
}
