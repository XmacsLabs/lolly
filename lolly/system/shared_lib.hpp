
/******************************************************************************
 * MODULE     : shared_lib.hpp
 * DESCRIPTION: dynamic (shared) library related routines
 * COPYRIGHT  : (C) 2024  jingkaimori
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#pragma once

#include "resource.hpp"
#include "url.hpp"
#include <stdint.h>
#include <tbox/tbox.h>

namespace lolly {
namespace system {

RESOURCE (shared_lib);

struct shared_lib_rep : rep<shared_lib> {
private:
  tb_dynamic_ref_t dynamic_ref;

public:
  shared_lib_rep (string dynamic_name, url path);
  virtual ~shared_lib_rep ();
  template <typename Ret, typename... Args>
  auto get_function (string function_name) -> Ret (*) (Args...) {
    return (Ret (*) (Args...)) tb_dynamic_func (dynamic_ref,
                                                c_string (function_name));
  }
  friend struct shared_lib;
};

shared_lib load_shared_library (string name, url path);

} // namespace system
} // namespace lolly
