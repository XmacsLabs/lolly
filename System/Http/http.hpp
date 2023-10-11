
/******************************************************************************
 * MODULE     : http.hpp
 * DESCRIPTION: HTTP related routines
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "tree.hpp"
#include "url.hpp"
#include <nlohmann/json.hpp>
using json= nlohmann::json;

namespace lolly {
namespace http {

json get (url u);

}
} // namespace lolly
