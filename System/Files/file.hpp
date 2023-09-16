
/******************************************************************************
 * MODULE     : file.hpp
 * DESCRIPTION: file handling
 * COPYRIGHT  : (C) 1999  Joris van der Hoeven
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#ifndef LOLLY_FILE_H
#define LOLLY_FILE_H

#include "url.hpp"

bool is_directory (url u);
bool is_regular (url u);
bool is_symbolic_link (url u);
bool is_newer (url which, url than);
bool is_of_type (url name, string filter);

int file_size (url u);
int last_modified (url u);

array<string> read_directory (url u, bool& error_flag);

void mkdir (url u);
void make_dir (url which);
void rmdir (url u);

url url_temp_dir ();
url url_temp (string suffix= "");

void remove (url u);
void move (url u1, url u2);
void copy (url u1, url u2);

bool   load_string (url file_name, string& s, bool fatal);
string string_load (url u);

#endif
