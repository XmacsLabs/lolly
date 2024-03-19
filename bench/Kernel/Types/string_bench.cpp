/** \file string_bench.cpp
 *  \copyright GPLv3
 *  \details Benchmark for string
 *  \author jingkaimori
 *  \date   2024
 */

#include "string.hpp"
#include "sys_utils.hpp"
#include <nanobench.h>

static ankerl::nanobench::Bench bench;

int
main () {
  lolly::init_tbox ();
#ifdef OS_WASM
  bench.minEpochIterations (2000);
#else
  bench.minEpochIterations (200000);
#endif
  bench.run ("string from int_16", [&] { as_string ((int16_t) 400); });
  bench.run ("string from int_32", [&] { as_string ((int32_t) 400); });
  bench.run ("string from int_64", [&] { as_string ((int64_t) 400); });
  bench.run ("string from double", [&] { as_string ((double) 400.0); });
  bench.run ("string to int", [&] { as_int ("400"); });
  bench.run ("string to long int", [&] { as_long_int ("400"); });
  bench.run ("string to double", [&] { as_double ("400.0"); });
}