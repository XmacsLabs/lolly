
/******************************************************************************
 * MODULE     : timer.hpp
 * DESCRIPTION: timer related routines
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "string.hpp"

namespace lolly {
namespace system {

void bench_start (string task);
void bench_cumul (string task);
void bench_end (tm_ostream& ostream, string task);
void bench_reset (string task);
void bench_print (tm_ostream& ostream, string task);
void bench_print (tm_ostream& ostream);

} // namespace system
} // namespace lolly
