
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
is_directory (string path) {
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
is_directory (url name) {
  string path= as_string (name);
  return is_directory (path);
}

void
mkdir (url u) {
  string label= u.label ();
  if (label == "" || label == "none" || label == "root" || label == "wildcard")
    return;
  else if (label == "concat") {
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
  if (label == "" || label == "none" || label == "root" || label == "wildcard")
    return;
  else if (label == "concat") {
    string path= as_string (u);
    tb_directory_remove (as_charp (path));
  }
  else { // label == "or"
    rmdir (u[1]);
    rmdir (u[2]);
  }
}
