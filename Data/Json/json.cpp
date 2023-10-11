#include "json.hpp"
#include "analyze.hpp"

namespace lolly {
namespace data {

string
json::to_string () {
  tree t= this->rep->t;
  if (is_atomic (t)) {
    return raw_quote (as_string (t));
  }
  if (t->op == INT64_TYPE || t->op == NULL_TYPE || t->op == BOOL_TYPE ||
      t->op == DOUBLE_TYPE) {
    return as_string (t[0]);
  }
  if (t->op == JSON_ARRAY) {
    string ret= "[";
    for (int i= 0; i < arity (t); i++) {
      ret= ret * json (t[0]).to_string ();
    }
    ret= ret * "]";
    return ret;
  }
  if (t->op == JSON_OBJECT) {
    string ret= "{";
    for (int i= 0; i < arity (t); i++) {
      ret= ret * raw_quote (as_string (t[0][0]));
      ret= ret * ":";
      ret= ret * json (t[0][1]).to_string ();
    }
    ret= ret * "}";
    return ret;
  }
}

} // namespace data
} // namespace lolly
