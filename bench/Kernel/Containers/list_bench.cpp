/** \file list_bench.cpp
 *  \copyright GPLv3
 *  \details Benchmark for list
 *  \author Darcy Shen
 *  \date   2024
 */

#include "list.hpp"
#include <nanobench.h>

static ankerl::nanobench::Bench bench;

static list<long>
gen (int64_t n) {
  auto normal= list<long> ();
  for (long i= 0; i < n; i++)
    normal << i;
  return normal;
}

int
main () {
  list<long> l1 = gen (1);
  list<long> l4 = gen (4);
  list<long> l16= gen (16);
  list<long> l32= gen (32);
  list<long> l32_2= gen (32);

  bench.run ("last_item 1", [&] { last_item (l1); });
  bench.run ("last_item 4", [&] { last_item (l4); });
  bench.run ("last_item 16", [&] { last_item (l16); });

  bench.run ("N 32", [&] { N (l32); });

  bench.run ("l1 == l2 32", [&] { l32 == l32_2; });
}
