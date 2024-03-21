#include "lolly/data/lolly_tree.hpp"
#include "lolly_doctests.hpp"

using namespace lolly::data;

TEST_CASE ("test construction of atomic tree") {
  string_eq (lolly_tree<int> (string_view ("label1"))->label, "label1");
  string_eq (lolly_tree<int> (string ("label2"))->label, "label2");
  string_eq (lolly_tree<int> ("label3")->label, "label3");
}