
/******************************************************************************
 * MODULE     : args.hpp
 * DESCRIPTION: lolly::args like nowide::args
 * COPYRIGHT  : (C) 2024  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

namespace lolly {
#if defined(OS_WIN) && defined(OS_MINGW)
class args {
public:
  ///
  /// Fix command line agruments
  ///
  args (int& argc, char**& argv)
      : old_argc_ (argc), old_argv_ (argv), old_argc_ptr_ (&argc),
        old_argv_ptr_ (&argv), {
    fix_args (argc, argv);
  }
  ///
  /// Restore original argc,argv,env values, if changed
  ///
  ~args () {
    if (old_argc_ptr_) *old_argc_ptr_= old_argc_;
    if (old_argv_ptr_) *old_argv_ptr_= old_argv_;
  }

private:
  void fix_args (int& argc, char**& argv) {
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
        if (!arg_values_[i].convert (wargv[i])) {
          wargc= i;
          break;
        }
        args_[i]= arg_values_[i].c_str ();
      }
      argc= wargc;
      argv= &args_[0];
    } catch (...) {
      LocalFree (wargv);
      throw;
    }
    LocalFree (wargv);
  }

  std::vector<char*>             args_;
  std::vector<short_stackstring> arg_values_;
  stackstring                    env_;
  std::vector<char*>             envp_;

  int    old_argc_;
  char** old_argv_;

  int*    old_argc_ptr_;
  char*** old_argv_ptr_;
};

#else
class args {
public:
  args (int&, char**&) {}
  ~args () {}
};
#endif

} // namespace lolly
