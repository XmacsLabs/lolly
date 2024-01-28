/** \file analyze_bench.cpp
 *  \copyright GPLv3
 *  \details Benchmark for string algorithms
 *  \author jingkaimori
 *  \date   2024
 */

#include "analyze.hpp"
#include "sys_utils.hpp"
#include <nanobench.h>

void
bench_string_join (string base_name, array<string> str) {
  ankerl::nanobench::Bench bench;
  bench.relative (true).minEpochIterations (1000).run (
      c_string (base_name * " by append"), [&] {
        string res;
        int    lth= N (str);
        for (size_t i= 0; i < lth; i++) {
          res << str[i];
        }
      });
  bench.run (c_string (base_name * " by concat"), [&] {
    string res;
    int    lth= N (str);
    for (size_t i= 0; i < lth; i++) {
      res= str[i] * res;
    }
  });
  bench.run (c_string (base_name * " by recompose"),
             [&] { recompose (str, ""); });
}

int
main () {
  lolly::init_tbox ();
  bench_string_join ("join regular string",
                     array<string> ("long string", "short", "", "<#ABCD>"));
  bench_string_join ("join two string", array<string> ("long string", "short"));
  bench_string_join ("join empty ones", array<string> (5));
  return 0;
}