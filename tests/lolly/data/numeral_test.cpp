
/******************************************************************************
 * MODULE     : numberal_test.cpp
 * DESCRIPTION: tests on numberal
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "a_tbox_main.cpp"
#include "lolly/data/numeral.hpp"

using Roman_nr= lolly::data::to_Roman;
using roman_nr= lolly::data::to_roman;

TEST_CASE ("roman_nr") {
  SUBCASE ("0-9") {
    string_eq (roman_nr (0), "o");
    string_eq (roman_nr (1), "i");
    string_eq (roman_nr (2), "ii");
    string_eq (roman_nr (3), "iii");
    string_eq (roman_nr (4), "iv");
    string_eq (roman_nr (5), "v");
    string_eq (roman_nr (6), "vi");
    string_eq (roman_nr (7), "vii");
    string_eq (roman_nr (8), "viii");
    string_eq (roman_nr (9), "ix");
    string_eq (roman_nr (-1), "-i");
    string_eq (roman_nr (-2), "-ii");
    string_eq (roman_nr (-3), "-iii");
    string_eq (roman_nr (-4), "-iv");
    string_eq (roman_nr (-5), "-v");
    string_eq (roman_nr (-6), "-vi");
    string_eq (roman_nr (-7), "-vii");
    string_eq (roman_nr (-8), "-viii");
    string_eq (roman_nr (-9), "-ix");
  }
  SUBCASE ("10-99") {
    string_eq (roman_nr (10), "x");
    string_eq (roman_nr (20), "xx");
    string_eq (roman_nr (30), "xxx");
    string_eq (roman_nr (40), "xl");
    string_eq (roman_nr (50), "l");
    string_eq (roman_nr (60), "lx");
    string_eq (roman_nr (70), "lxx");
    string_eq (roman_nr (80), "lxxx");
    string_eq (roman_nr (90), "xc");
    string_eq (roman_nr (99), "xcix");
    string_eq (roman_nr (-10), "-x");
    string_eq (roman_nr (-20), "-xx");
    string_eq (roman_nr (-30), "-xxx");
    string_eq (roman_nr (-40), "-xl");
    string_eq (roman_nr (-50), "-l");
    string_eq (roman_nr (-60), "-lx");
    string_eq (roman_nr (-70), "-lxx");
    string_eq (roman_nr (-80), "-lxxx");
    string_eq (roman_nr (-90), "-xc");
    string_eq (roman_nr (-99), "-xcix");
  }
  SUBCASE ("100-999") {
    string_eq (roman_nr (100), "c");
    string_eq (roman_nr (200), "cc");
    string_eq (roman_nr (300), "ccc");
    string_eq (roman_nr (400), "cd");
    string_eq (roman_nr (500), "d");
    string_eq (roman_nr (600), "dc");
    string_eq (roman_nr (700), "dcc");
    string_eq (roman_nr (800), "dccc");
    string_eq (roman_nr (900), "cm");
    string_eq (roman_nr (999), "cmxcix");
    string_eq (roman_nr (-100), "-c");
    string_eq (roman_nr (-200), "-cc");
    string_eq (roman_nr (-300), "-ccc");
    string_eq (roman_nr (-400), "-cd");
    string_eq (roman_nr (-500), "-d");
    string_eq (roman_nr (-600), "-dc");
    string_eq (roman_nr (-700), "-dcc");
    string_eq (roman_nr (-800), "-dccc");
    string_eq (roman_nr (-900), "-cm");
    string_eq (roman_nr (-999), "-cmxcix");
  }
  SUBCASE ("1000-3999") {
    string_eq (roman_nr (3999), "mmmcmxcix");
    string_eq (roman_nr (-3999), "-mmmcmxcix");
  }
  SUBCASE ("max int32 or min int32") {
    string_eq (roman_nr (4000), "?");
    string_eq (roman_nr (0x7FFFFFFF), "?");
    string_eq (roman_nr (0x80000000), "?");
  }
}
