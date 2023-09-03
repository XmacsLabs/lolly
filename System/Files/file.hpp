
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

int file_size (url u);

array<string> read_directory (url u, bool& error_flag);

void mkdir (url u);
void rmdir (url u);

#endif
