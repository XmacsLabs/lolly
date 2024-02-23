
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
#include "analyze.hpp"
#include "tbox/tbox.h"

namespace lolly {
namespace system {

array<string>
parse_command_line (string cmd) {
  array<string> ret       = array<string> ();
  bool          quote_mode= false;
  int           i= 0, cmd_N= N (cmd), j= 0;
  while (i < cmd_N) {
    if (quote_mode) {
      // i>0 because is_quoted is false at the beginning
      int next_i= index_of (cmd, '"', i);
      if (next_i == -1) {
        // no ending '"', just commit the cmd part and quite loop
        i= cmd_N;
        ret << cmd (j, i);
      }
      else if (cmd[next_i - 1] == '\\') {
        // skip when there is '\\' before '"'
        i= next_i + 1;
      }
      else {
        // commit the cmd part and reset the cmd part index j
        i= next_i + 1;
        ret << cmd (j, i);
        j= i;
        // quite quote mode
        quote_mode= false;
      }
    }
    else {
      char c= cmd[i];
      if (c == '"') {
        // enter the quote mode and reset the cmd part index j
        quote_mode= true;
        j         = i;
        i++;
      }
      else if (c == ' ') {
        if (i == j) {
          // skip the space
          i++;
          j++;
        }
        else {
          // commit the cmd part and reset the cmd part index j
          ret << cmd (j, i);
          i++;
          j= i;
        }
      }
      else {
        i++;
        if (i == cmd_N) {
          ret << cmd (j, i);
        }
      }
    }
  }

  return ret;
}

int
call (string cmd) {
#ifdef OS_WASM
  return -1;
#else
  tb_process_attr_t attr= {0};
  attr.flags            = TB_PROCESS_FLAG_NO_WINDOW;

  array<string> arr  = parse_command_line (cmd);
  int           arr_N= N (arr);
  if (arr_N == 0 || arr_N >= 101) return -1;

  const tb_char_t* args[100]= {tb_null};
  for (int i= 0; i < arr_N; i++) {
    args[i]= (char*) c_string (arr[i]);
  }
  const tb_char_t** argv   = &args[0];
  int               retcode= tb_process_run (argv[0], argv, &attr);

  return retcode;
#endif
}

int
check_output (string cmd, string& result, bool stderr_only, int64_t timeout) {
  tb_long_t status= -1;
  // init pipe files
  tb_pipe_file_ref_t file[2]= {0};
  if (!tb_pipe_file_init_pair (file, tb_null, 0)) {
    return status;
  }

  // init process
  tb_process_attr_t attr= {0};
  attr.flags            = TB_PROCESS_FLAG_NO_WINDOW;
  if (stderr_only) {
    attr.err.pipe= file[1];
    attr.errtype = TB_PROCESS_REDIRECT_TYPE_PIPE;
  }
  else {
    attr.out.pipe= file[1];
    attr.outtype = TB_PROCESS_REDIRECT_TYPE_PIPE;
  }

  array<string> arr  = parse_command_line (cmd);
  int           arr_N= N (arr);
  if (arr_N == 0 || arr_N >= 101) return -1;

  tb_process_ref_t process;
  const tb_char_t* args[100]= {0};
  for (int i= 0; i < arr_N; i++) {
    args[i]= (char*) c_string (arr[i]);
  }
  const tb_char_t** argv= &args[0];
  process               = tb_process_init (argv[0], argv, &attr);

  if (process) {
    // read pipe data
    tb_size_t read= 0;
    // TODO: should be a config here
    tb_byte_t data[8192]= {0};
    tb_size_t size      = sizeof (data);
    tb_bool_t wait      = tb_false;
    while (read < size) {
      tb_long_t real= tb_pipe_file_read (file[0], data + read, size - read);
      if (real > 0) {
        read+= real;
        wait= tb_false;
      }
      else if (!real && !wait) {
        tb_long_t ok   = 0;
        int       retry= 25;
        if (read > 0) {
          retry= 2;
        }
        while (retry > 0 && (ok == 0)) {
          ok   = tb_pipe_file_wait (file[0], TB_PIPE_EVENT_READ, timeout);
          retry= retry - 1;
        }
        tb_check_break (ok > 0);
        wait= tb_true;
      }
      else break;
    }

    result= as_string ((tb_char_t*) data);

    // wait process
    tb_process_wait (process, &status, timeout);

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