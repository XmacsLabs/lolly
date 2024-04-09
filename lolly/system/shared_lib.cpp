
#include "shared_lib.hpp"

namespace lolly {
namespace system {

shared_lib_rep::shared_lib_rep (string dynamic_name, url path)
    : dynamic_ref (tb_dynamic_init (c_string (as_system_string (path)))),
      rep (dynamic_name){};
shared_lib_rep::~shared_lib_rep () {
  tb_dynamic_exit (dynamic_ref);
  shared_lib::instances->reset (res_name);
};

shared_lib
load_shared_library (string name, url path) {
  return make (shared_lib, name, tm_new<shared_lib_rep> (name, path));
}

} // namespace system
} // namespace lolly
RESOURCE_CODE (lolly::system::shared_lib);
