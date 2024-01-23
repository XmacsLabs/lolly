#include "args.hpp"
#include "lolly/data/unicode.hpp"
#include "string.hpp"

#include <shellapi.h>
#include <stdio.h>
#include <windows.h>

using lolly::data::wchar_to_utf8;

namespace lolly {
namespace system {
void
args::fix_args (int& argc, char**& argv) {
  int       wargc;
  wchar_t** wargv= CommandLineToArgvW (GetCommandLineW (), &wargc);
  if (!wargv) {
    argc              = 0;
    static char* dummy= 0;
    argv              = &dummy;
    return;
  }
  try {
    args_.resize (wargc + 1, 0);
    arg_values_.resize (wargc);
    for (int i= 0; i < wargc; i++) {
      arg_values_[i]= wchar_to_utf8 (wargv[i]);
      args_[i]      = as_charp (arg_values_[i]);
    }
    argc= wargc;
    argv= &args_[0];
  } catch (...) {
    LocalFree (wargv);
    throw;
  }
  LocalFree (wargv);
}

} // namespace system
} // namespace lolly
