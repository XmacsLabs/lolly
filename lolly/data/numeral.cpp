
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

} // namespace data
} // namespace lolly
