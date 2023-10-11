
#include "a_lolly_test.hpp"
#include "json.hpp"
#include "string.hpp"

using lolly::data::as_string;
using lson = lolly::data::json;

TEST_CASE ("json access") {
    lson l= lson();
    l.set("name", "Bob");
    string_eq (as_string(l("name")), "Bob");
}
