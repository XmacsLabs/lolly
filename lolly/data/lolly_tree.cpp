#include "lolly_tree.hpp"

namespace lolly {
namespace data {

template <typename T>
lolly_tree<T>
copy (lolly_tree<T> t) {
  if (is_atomic (t)) return tree (copy (t->label));
  else {
    int           i, n= N (t);
    lolly_tree<T> t2 (t, n);
    for (i= 0; i < n; i++)
      t2[i]= copy (t[i]);
    return t2;
  }
}

} // namespace data
} // namespace lolly
