/** \file numeral_bench.cpp
 *  \copyright GPLv3
 *  \details Benchmark for converter between numeral and string
 *  \author jingkaimori
 *  \date   2024
 */

#include "lolly/data/numeral.hpp"
#include <nanobench.h>

using namespace lolly::data;

static ankerl::nanobench::Bench bench;

namespace lolly {
extern void init_tbox ();
} // namespace lolly

int
main () {
  lolly::init_tbox ();
#ifdef OS_WASM
  bench.minEpochIterations (2000);
#else
  bench.minEpochIterations (400000);
#endif

  string hex_string;
  for (int d= 1; d < 6; d++) {
    hex_string << ('a' + d % 6);
    bench.complexityN (d);
    bench.run ("convert hexadecimal string to int",
               [&] { from_hex (hex_string); });
    int hex_number= (0x1 << (d + 1)) - 1;
    bench.run ("convert signed int to hexadecimal string",
               [&] { to_Hex (hex_number); });
    bench.run ("convert unsigned int to hexadecimal string",
               [&] { as_hexadecimal (hex_number, d); });
  }
}
