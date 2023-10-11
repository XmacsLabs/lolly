
/******************************************************************************
 * MODULE     : json.hpp
 * DESCRIPTION: JSON Data
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "string.hpp"
#include "tree.hpp"

namespace lolly {
namespace data {

enum json_label : int {
  STRING_TYPE= 0,
  NULL_TYPE,
  BOOL_TYPE,
  INT64_TYPE,
  UINT64_TYPE,
  DOUBLE_TYPE,
  JSON_OBJECT,
  JSON_ARRAY,
  JSON_PAIR
};

struct json_rep : concrete_struct {
  tree t;
  inline json_rep (tree p_t) : t (p_t) {}
};

class json {
  CONCRETE (json);

private:
  json (tree t) : rep (tm_new<json_rep> (t)) {}

public:
  // empty json
  json () { rep= tm_new<json_rep> (tree (JSON_OBJECT)); };

  // primitives constructors
  json (string value) { rep= tm_new<json_rep> (tree (value)); }
  json (nullptr_t value) { rep= tm_new<json_rep> (tree (NULL_TYPE)); }
  json (bool value) {
    rep= tm_new<json_rep> (tree (BOOL_TYPE, tree (as_string (value))));
  }
  json (DI value) {
    rep= tm_new<json_rep> (tree (INT64_TYPE, tree (as_string (value))));
  }
  json (double value) { rep= tm_new<json_rep> (tree (as_string (value))); }
  json (array<json> value) {
    array<tree> arr= array<tree> ();
    for (int i= 0; i < N (value); i++) {
      arr << value[i]->t;
    }
    rep= tm_new<json_rep> (tree (JSON_ARRAY, arr));
  }

  void set (string key, json value) {
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

  json operator() (string key) {
    if (rep->t->op == JSON_OBJECT) {
      int n= arity (rep->t);
      for (int i= 0; i < n; i++) {
        tree iter= rep->t[i];
        if (is_atomic (iter[0]) && iter[0] == key) {
          return json (iter[1]);
        }
      }
    }
    return json (nullptr);
  }

  bool is_null () { return rep->t->op == NULL_TYPE; }

  string to_string ();
};
CONCRETE_CODE (json);

} // namespace data
} // namespace lolly
