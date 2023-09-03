
/******************************************************************************
* MODULE     : file.cpp
* DESCRIPTION: file handling
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
                   2023  Darcy Shen
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "file.hpp"
#include "string.hpp"
#include "tbox/tbox.h"

static bool
is_single_path (url u) {
  string label= u.label ();
  return (label == "") || (label == "concat");
}

bool
is_directory (url u) {
  if (!is_single_path (u)) return false;

  string         path= as_string (u);
  tb_file_info_t info;
  if (tb_file_info (as_charp (path), &info)) {
    switch (info.type) {
    case TB_FILE_TYPE_DIRECTORY:
      return true;
    default:
      return false;
    }
  }
  else {
    return false;
  }
}

bool
is_regular (url u) {
  if (!is_single_path (u)) return false;

  string         path= as_string (u);
  tb_file_info_t info;
  if (tb_file_info (as_charp (path), &info)) {
    switch (info.type) {
    case TB_FILE_TYPE_FILE:
      return true;
    default:
      return false;
    }
  }
  else {
    return false;
  }
}

bool
is_symbolic_link (url u) {
  if (!is_single_path (u)) return false;

  string         path= as_string (u);
  tb_file_info_t info;
  if (tb_file_info (as_charp (path), &info)) {
    return (info.flags | TB_FILE_FLAG_LINK) != 0;
  }
  else {
    return false;
  }
}

int
file_size (url u) {
  if (!is_single_path (u)) return -1;

  string         path= as_string (u);
  tb_file_info_t info;
  if (tb_file_info (as_charp (path), &info)) {
    return info.size;
  }
  else {
    return -1;
  }
}

static tb_long_t
tb_directory_walk_func (tb_char_t const* path, tb_file_info_t const* info,
                        tb_cpointer_t priv) {
  // check
  tb_assert_and_check_return_val (path && info, TB_DIRECTORY_WALK_CODE_END);

  array<string>* p_arr_result= (array<string>*) priv;
  (*p_arr_result) << as_string (tail (url_system (string (path))));
  return TB_DIRECTORY_WALK_CODE_CONTINUE;
}

array<string>
read_directory (url u, bool& error_flag) {
  if (!is_single_path (u)) {
    error_flag= false;
    return array<string> ();
  }

  string        path      = as_string (u);
  array<string> arr_result= array<string> ();
  error_flag              = !is_directory (u);
  if (error_flag) {
    return arr_result;
  }
  tb_directory_walk (as_charp (path), 0, tb_false, tb_directory_walk_func,
                     &arr_result);
  return arr_result;
}

void
mkdir (url u) {
  string label= u.label ();
  if (label == "none" || label == "root" || label == "wildcard") return;
  else if (is_single_path (u)) { // label == "" or label == "concat"
    string path= as_string (u);
    tb_directory_create (as_charp (path));
  }
  else { // label == "or"
    mkdir (u[1]);
    mkdir (u[2]);
  }
}

void
rmdir (url u) {
  string label= u.label ();
  if (label == "none" || label == "root" || label == "wildcard") return;
  else if (is_single_path (u)) {
    string path= as_string (u);
    tb_directory_remove (as_charp (path));
  }
  else { // label == "or"
    rmdir (u[1]);
    rmdir (u[2]);
  }
}
