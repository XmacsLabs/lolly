
/******************************************************************************
 * MODULE     : http_response.hpp
 * DESCRIPTION: Tree repr of HTTP Response
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "tree.hpp"

enum http_response_label : int {
  STATUS_CODE= 1,
  TEXT,
  URL,
  ELAPSED,
  HEADER,
  ROOT,
  TUPLE
};
