#include "lolly/system/args.hpp"
#include "string.hpp"
#include "sys_utils.hpp"

int
main (int argc, char** argv) {
  lolly::init_tbox ();
  lolly::system::args a (argc, argv);
  cout << argc << LF;
  cout << string (argv[0]) << LF;
  if (argc > 1) {
    cout << string (argv[1]) << LF;
  }
}
