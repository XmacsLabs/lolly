
/******************************************************************************
 * MODULE     : numberal.hpp
 * DESCRIPTION:
 * COPYRIGHT  : (C) 2013  Francois Poulain
 *                  2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "numeral.hpp"
#include "analyze.hpp"

namespace lolly {
namespace data {
static string roman_ones[10]    = {"",  "i",  "ii",  "iii",  "iv",
                                   "v", "vi", "vii", "viii", "ix"};
static string roman_tens[10]    = {"",  "x",  "xx",  "xxx",  "xl",
                                   "l", "lx", "lxx", "lxxx", "xc"};
static string roman_hundreds[10]= {"",  "c",  "cc",  "ccc",  "cd",
                                   "d", "dc", "dcc", "dccc", "cm"};
static string roman_thousands[4]= {"", "m", "mm", "mmm"};

string
to_roman (int32_t nr) {
  if (nr == 0) return "o";
  if (nr > 3999 || nr < -3999) return "?";
  if (nr < 0) return "-" * to_roman (-nr);
  return roman_thousands[(nr / 1000) % 10] * roman_hundreds[(nr / 100) % 10] *
         roman_tens[(nr / 10) % 10] * roman_ones[nr % 10];
}

string
to_Roman (int32_t nr) {
  return upcase_all (to_roman (nr));
}

// 0 should not be used as index of this array, or bug occurs. because digit 0
// is handled specially according to position of digit.
static string chars_han[10]= {
    "<unspecified>", "一", "二", "三", "四", "五", "六", "七", "八", "九"};

string
hanzi_sub (int16_t nr, bool leading_zero) {
  short thousand= (nr % 10000) / 1000, hundred= (nr % 1000) / 100,
        ten= (nr % 100) / 10, one= nr % 10;
  short cases= (leading_zero << 4) | ((thousand == 0) << 3) |
               ((hundred == 0) << 2) | ((ten == 0) << 1) | (one == 0);
  switch (cases) {
  case 0x0:
  case 0x10:
    return chars_han[thousand] * "千" * chars_han[hundred] * "百" *
           chars_han[ten] * "十" * chars_han[one];
  case 0x1:
  case 0x11:
    return chars_han[thousand] * "千" * chars_han[hundred] * "百" *
           chars_han[ten] * "十";
  case 0x2:
  case 0x12:
    return chars_han[thousand] * "千" * chars_han[hundred] * "百零" *
           chars_han[one];
  case 0x3:
  case 0x13:
    return chars_han[thousand] * "千" * chars_han[hundred] * "百";
  case 0x4:
  case 0x14:
    return chars_han[thousand] * "千零" * chars_han[ten] * "十" *
           chars_han[one];
  case 0x5:
  case 0x15:
    return chars_han[thousand] * "千零" * chars_han[ten] * "十";
  case 0x6:
  case 0x16:
    return chars_han[thousand] * "千零" * chars_han[one];
  case 0x7:
  case 0x17:
    return chars_han[thousand] * "千";
  case 0x8:
    return chars_han[hundred] * "百" * chars_han[ten] * "十" * chars_han[one];
  case 0x18:
    return "零" * chars_han[hundred] * "百" * chars_han[ten] * "十" *
           chars_han[one];
  case 0x9:
    return chars_han[hundred] * "百" * chars_han[ten] * "十";
  case 0x19:
    return "零" * chars_han[hundred] * "百" * chars_han[ten] * "十";
  case 0xA:
    return chars_han[hundred] * "百零" * chars_han[one];
  case 0x1A:
    return "零" * chars_han[hundred] * "百零" * chars_han[one];
  case 0xB:
    return chars_han[hundred] * "百";
  case 0x1B:
    return "零" * chars_han[hundred] * "百";
  case 0xC:
    if (ten == 1) {
      return "十" * chars_han[one];
    }
    else {
      return chars_han[ten] * "十" * chars_han[one];
    }
  case 0x1C:
    return "零" * chars_han[ten] * "十" * chars_han[one];
  case 0xD:
    if (ten == 1) {
      return "十";
    }
    else {
      return chars_han[ten] * "十";
    }
  case 0x1D:
    return "零" * chars_han[ten] * "十";
  case 0xE:
    return chars_han[one];
  case 0x1E:
    return "零" * chars_han[one];
  case 0xF:
  case 0x1F:
    return "";
  default:
    return "<unspecified>" * as_string (cases);
  }
}

string
hanzi_nr (int32_t nr) {
  if (nr == 0) return "零";
  if (nr == 0x80000000) return "负二十一亿四千七百四十八万三千六百四十八";
  if (nr < 0) return "负" * hanzi_nr (-nr);
  if (nr >= 100000000) {
    return hanzi_sub (nr / 100000000, false) * "亿" *
           hanzi_sub ((nr / 10000) % 10000, true) * "万" *
           hanzi_sub (nr % 10000, true);
  }
  if (nr >= 10000) {
    return hanzi_sub (nr / 10000, false) * "万" * hanzi_sub (nr % 10000, true);
  }
  return hanzi_sub (nr, false);
}


} // namespace data
} // namespace lolly
