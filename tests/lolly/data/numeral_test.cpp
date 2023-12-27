
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

using namespace lolly::data;

TEST_CASE ("to_roman") {
  SUBCASE ("0-9") {
    string_eq (to_roman (0), "o");
    string_eq (to_roman (1), "i");
    string_eq (to_roman (2), "ii");
    string_eq (to_roman (3), "iii");
    string_eq (to_roman (4), "iv");
    string_eq (to_roman (5), "v");
    string_eq (to_roman (6), "vi");
    string_eq (to_roman (7), "vii");
    string_eq (to_roman (8), "viii");
    string_eq (to_roman (9), "ix");
    string_eq (to_roman (-1), "-i");
    string_eq (to_roman (-2), "-ii");
    string_eq (to_roman (-3), "-iii");
    string_eq (to_roman (-4), "-iv");
    string_eq (to_roman (-5), "-v");
    string_eq (to_roman (-6), "-vi");
    string_eq (to_roman (-7), "-vii");
    string_eq (to_roman (-8), "-viii");
    string_eq (to_roman (-9), "-ix");
  }
  SUBCASE ("10-99") {
    string_eq (to_roman (10), "x");
    string_eq (to_roman (20), "xx");
    string_eq (to_roman (30), "xxx");
    string_eq (to_roman (40), "xl");
    string_eq (to_roman (50), "l");
    string_eq (to_roman (60), "lx");
    string_eq (to_roman (70), "lxx");
    string_eq (to_roman (80), "lxxx");
    string_eq (to_roman (90), "xc");
    string_eq (to_roman (99), "xcix");
    string_eq (to_roman (-10), "-x");
    string_eq (to_roman (-20), "-xx");
    string_eq (to_roman (-30), "-xxx");
    string_eq (to_roman (-40), "-xl");
    string_eq (to_roman (-50), "-l");
    string_eq (to_roman (-60), "-lx");
    string_eq (to_roman (-70), "-lxx");
    string_eq (to_roman (-80), "-lxxx");
    string_eq (to_roman (-90), "-xc");
    string_eq (to_roman (-99), "-xcix");
  }
  SUBCASE ("100-999") {
    string_eq (to_roman (100), "c");
    string_eq (to_roman (200), "cc");
    string_eq (to_roman (300), "ccc");
    string_eq (to_roman (400), "cd");
    string_eq (to_roman (500), "d");
    string_eq (to_roman (600), "dc");
    string_eq (to_roman (700), "dcc");
    string_eq (to_roman (800), "dccc");
    string_eq (to_roman (900), "cm");
    string_eq (to_roman (999), "cmxcix");
    string_eq (to_roman (-100), "-c");
    string_eq (to_roman (-200), "-cc");
    string_eq (to_roman (-300), "-ccc");
    string_eq (to_roman (-400), "-cd");
    string_eq (to_roman (-500), "-d");
    string_eq (to_roman (-600), "-dc");
    string_eq (to_roman (-700), "-dcc");
    string_eq (to_roman (-800), "-dccc");
    string_eq (to_roman (-900), "-cm");
    string_eq (to_roman (-999), "-cmxcix");
  }
  SUBCASE ("1000-3999") {
    string_eq (to_roman (3999), "mmmcmxcix");
    string_eq (to_roman (-3999), "-mmmcmxcix");
  }
  SUBCASE ("max int32 or min int32") {
    string_eq (to_roman (4000), "?");
    string_eq (to_roman (0x7FFFFFFF), "?");
    string_eq (to_roman (0x80000000), "?");
  }
}

TEST_CASE ("to_hanzi") {
  string_eq (to_hanzi (-1605), "负一千六百零五");
  string_eq (to_hanzi (0), "零");
  string_eq (to_hanzi (1), "一");
  string_eq (to_hanzi (10), "十");
  string_eq (to_hanzi (11), "十一");
  string_eq (to_hanzi (42), "四十二");
  string_eq (to_hanzi (90), "九十");
  string_eq (to_hanzi (100), "一百");
  string_eq (to_hanzi (102), "一百零二");
  string_eq (to_hanzi (110), "一百一十");
  string_eq (to_hanzi (123), "一百二十三");
  string_eq (to_hanzi (1000), "一千");
  string_eq (to_hanzi (1001), "一千零一");
  string_eq (to_hanzi (1024), "一千零二十四");
  string_eq (to_hanzi (1030), "一千零三十");
  string_eq (to_hanzi (1600), "一千六百");
  string_eq (to_hanzi (1605), "一千六百零五");
  string_eq (to_hanzi (10000), "一万");
  string_eq (to_hanzi (10001), "一万零一");
  string_eq (to_hanzi (153457), "十五万三千四百五十七");
  string_eq (to_hanzi (300153457), "三亿零一十五万三千四百五十七");
  string_eq (to_hanzi (0x7FFFFFFF), "二十一亿四千七百四十八万三千六百四十七");
  string_eq (to_hanzi (0x80000000), "负二十一亿四千七百四十八万三千六百四十八");
}

TEST_CASE ("as_hex") {
  SUBCASE ("0~255") {
    string_eq (to_padded_hex ((uint8_t) 0), "00");
    string_eq (to_padded_hex ((uint8_t) 1), "01");
    string_eq (to_padded_hex ((uint8_t) 255), "ff");
  }
  SUBCASE ("overflow") {
    string_eq (to_padded_hex ((uint8_t) -1), "ff");
    string_eq (to_padded_hex ((uint8_t) 256), "00");
  }
}
