
/******************************************************************************
 * MODULE     : subprocess.cpp
 * DESCRIPTION: subprocess related routines
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "lolly/system/subprocess.hpp"
#include "tbox/tbox.h"

namespace lolly {
namespace system {

int
call (string cmd) {
#ifdef OS_WASM
  return -1;
#else
  tb_process_attr_t attr= {0};
  attr.flags            = TB_PROCESS_FLAG_NO_WINDOW;
  c_string cmd_ (cmd);
  return (int) tb_process_run_cmd (cmd_, &attr);
#endif
}

int
check_output (string s, string& result, int64_t timeout) {
  tb_long_t status= -1;
  // init pipe files
  tb_pipe_file_ref_t file[2]= {0};
  if (!tb_pipe_file_init_pair (file, tb_null, 0)) {
    return status;
  }

  // init process
  c_string          cmd_ (s);
  tb_process_attr_t attr  = {0};
  attr.out.pipe           = file[1];
  attr.outtype            = TB_PROCESS_REDIRECT_TYPE_PIPE;
  attr.flags              = TB_PROCESS_FLAG_NO_WINDOW;
  tb_process_ref_t process= tb_process_init_cmd (cmd_, &attr);
  if (process) {
    // read pipe data
    tb_size_t read= 0;
    // TODO: should be a config here
    tb_byte_t data[8192];
    tb_size_t size= sizeof (data);

    int retry= 3;
    while (read < size) {
      tb_long_t real= tb_pipe_file_read (file[0], data + read, size - read);
      cout << "real\t" << real << LF;
      if (real > 0) {
        read+= real;
      }
      tb_long_t ok= tb_pipe_file_wait (file[0], TB_PIPE_EVENT_READ, timeout);
      if (ok == 0) { // timeout
        retry= retry - 1;
        cout << "retry: " << retry << LF;
        if (retry == 0) break;
      }
      else if (ok == -1) { // failed
        break;
      }
    }

    result= as_string ((tb_char_t*) data);

    // wait process
    tb_process_wait (process, &status, -1);

    // exit process
    tb_process_exit (process);
  }

  // exit pipe files
  tb_pipe_file_exit (file[0]);
  tb_pipe_file_exit (file[1]);
  return status;
}

} // namespace system
} // namespace lolly