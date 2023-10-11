#include "json.hpp"
#include "analyze.hpp"

namespace lolly {
namespace data {

json
json::operator() (string key) {
  if (rep->t->op == JSON_OBJECT) {
    int n= arity (rep->t);
    for (int i= 0; i < n; i++) {
      tree iter= rep->t[i];
      if (is_atomic (iter[0]) && iter[0] == key) {
        return json (iter[1]);
      }
    }
  }
  return json_null ();
}

json
json::get (string key) {
  return (*this) (key);
}

void
json::set (string key, json value) {
  json old_v= (*this) (key);
  if (old_v.is_null ()) {
    rep->t << tree (JSON_PAIR, key, value->t);
  }
  else {
    int n= arity (rep->t);
    for (int i= 0; i < n; i++) {
      tree iter= rep->t[i];
      if (is_atomic (iter[0]) && iter[0] == key) {
        iter[1]= value->t;
      }
    }
  }
}

string
json::to_string () {
  tree t= this->rep->t;
  if (this->is_string ()) {
    return raw_quote (as_string (t));
  }
  if (this->is_null ()) {
    return raw_quote ("null");
  }
  if (this->is_bool ()) {
    return raw_quote (as_string (t[0]));
  }
  if (this->is_number ()) {
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
    int    n  = arity (t);
    if (n > 0) {
      ret= ret * raw_quote (as_string (t[0][0]));
      ret= ret * ":";
      ret= ret * json (t[0][1]).to_string ();
    }
    for (int i= 1; i < n; i++) {
      ret= ret * ",";
      ret= ret * raw_quote (as_string (t[0][0]));
      ret= ret * ":";
      ret= ret * json (t[i][1]).to_string ();
    }
    ret= ret * "}";
    return ret;
  }
}

string
as_string (json j) {
  if (j.is_string ()) {
    return as_string (j->t);
  }
  TM_FAILED ("not a string");
}

} // namespace data
} // namespace lolly
