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
  bench.run ("construct string", [&] {
    string ("abc");
    string ();
  });
  bench.run ("equality of string", [&] {
    static string a ("abc"), b;
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
    static string a ("abcde");
    a (2, 3);
  });
  bench.minEpochIterations (40000);
  bench.run ("concat string", [&] {
    static string a ("abc"), b ("de");
    a*            b;
  });
  bench.run ("append string", [&] {
    static string a ("abc"), b ("de");
    a << b;
  });
  bench.run ("is quoted", [&] {
    static string a ("H\"ello TeXmacs\"");
    is_quoted (a);
  });
}