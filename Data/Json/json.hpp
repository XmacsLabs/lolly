
/******************************************************************************
 * MODULE     : json.hpp
 * DESCRIPTION: JSON Data
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "tree.hpp"
#include <cstdint>

namespace lolly {
namespace json {

enum json_label : int {
  STRING_TYPE= 0,
  BOOL_TYPE,
  INT64_TYPE,
  UINT64_TYPE,
  DOUBLE_TYPE,
  JSON_OBJECT,
  JSON_ARRAY,
  JSON_PAIR,
};

struct json_rep : concrete_struct {
  tree t;
  inline tree_rep (tree p_t) : t (p_t) {}
};

class json {
  CONCRETE (json);

  // empty json
  json ();

  // primitives
  json (string value) { rep= tm_new<json_rep> (tree (value)); }
  json (bool value) {
    rep= tm_new<json_rep> (tree (BOOL_TYPE), tree (as_string (value)));
  }
  json (int64_t value) {
    rep= tm_new<json_rep> (tree (INT64_TYPE, tree (as_string (value))));
  }
  json (double value) { rep= tm_new<json_rep> (tree (as_string (value))); }

  // pair
  json (string key, json value) {
    rep= tm_new<json_rep> (
        tree (static_cast<int> (JSON_OBJECT),
              tree (static_cast<int> (JSON_PAIR), tree (key), value->t)));
  }

  // json operator () (string key) {

  //}
}

} // namespace json
} // namespace lolly
