/** \file url_bench.cpp
 *  \copyright GPLv3
 *  \details Benchmark for url
 *  \author Darcy Shen
 *  \date   2024
 */

#include "sys_utils.hpp"
#include "url.hpp"
#include <nanobench.h>

static ankerl::nanobench::Bench bench;

int
main () {
  lolly::init_tbox ();
  bench.run ("url descends equal",
             [&] { descends (url ("a/b/c"), url ("a/b/c")); });
  bench.run ("url descends concat",
             [&] { descends (url ("a/b/c"), url ("a/b/c/d/e/f")); });
}
