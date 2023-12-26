
/******************************************************************************
 * MODULE     : numberal.hpp
 * DESCRIPTION: convertions for various numberal systems
 * COPYRIGHT  : (C) 2013  Francois Poulain
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "string.hpp"
#include <stdint.h>

namespace lolly {
namespace data {
/**
 * @brief Generates a Roman numeral string for an integer.
 *
 * @param nr The integer to be converted to Roman numeral.
 * @return A string representing the Roman numeral.
 */
string to_roman (int32_t nr);

/**
 * @brief Generates an uppercase Roman numeral string for an integer.
 *
 * @param nr The integer to be converted to Roman numeral.
 * @return A string representing the uppercase Roman numeral.
 */
string to_Roman (int32_t nr);

/**
 * @brief Generates a Chinese numeral for a given integer.
 *
 * @param nr The integer to be converted to a Chinese numeral.
 * @return A string representing the Chinese numeral.
 */
string to_hanzi (int32_t nr);
} // namespace data
} // namespace lolly
