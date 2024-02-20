/** \file string_bench.cpp
 *  \copyright GPLv3
 *  \details Benchmark for string
 *  \author jingkaimori
 *  \date   2024
 */

#include "lolly/data/tm_string.hpp"
#include <nanobench.h>

static ankerl::nanobench::Bench bench;

namespace lolly {
extern void init_tbox ();
} // namespace lolly
using lolly::data::string;

int
main () {
  lolly::init_tbox ();
  bench.minEpochIterations (200000).run ("construct string", [&] {
    string ("abc");
    string ();
  });
  bench.run ("equality of string", [&] {
    static string a ("abc"), b;
    a == b;
  });
  bench.run ("equality of larger string", [&] {
    static string a ("equality of larger string"),
        b ("equality of larger strinG");
    a == b;
  });
  bench.run ("compare string", [&] {
    static string a ("ab"), b ("b");
    a <= b;
  });
  bench.run ("compare larger string", [&] {
    static string a ("compare larger string"), b ("compare LARGER string");
    a <= b;
  });
  bench.run ("slice string", [&] {
    static string a ("abcdefgh");
    a (2, 3);
  });
  bench.run ("slice string with larger range", [&] {
    static string a ("abcdefgh");
    a (1, 6);
  });
  bench.run ("concat string", [&] {
    static string a ("abc"), b ("de");
    a*            b;
  });
  bench.run ("append string", [&] {
    static string a ("abc"), b ("de");
    a << b;
  });
}