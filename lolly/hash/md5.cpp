
/******************************************************************************
 * MODULE     : md5.cpp
 * DESCRIPTION: md5 digest
 * COPYRIGHT  : (C) 2023  Darcy Shen
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include <tbox/tbox.h>

#include "file.hpp"
#include "md5.hpp"

namespace lolly {
namespace hash {
string
md5_hexdigest (url u) {
  if (!is_local_and_single (u)) {
    return string ("");
  }
  string      name= as_string (u);
  const char* path= as_charp (name);
  if (!tb_file_access (path, TB_FILE_MODE_RO)) {
    return string ("");
  }
  tb_file_ref_t file= tb_file_init (path, TB_FILE_MODE_RO);
  if (file == tb_null) {
    return string ("");
  }
  tb_size_t i_size= tb_file_size (file);
  if (i_size == 0) {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }
  tb_file_sync (file); // lock file
  tb_byte_t* i_buffer   = tm_new_array<tb_byte_t> (i_size);
  tb_size_t  real_size  = tb_file_read (file, i_buffer, i_size);
  bool       read_sz_equ= (real_size == i_size);
  bool       exit_suc   = tb_file_exit (file); // exit file
  if (read_sz_equ && exit_suc) {
    tb_byte_t o_buffer[16];
    tb_size_t o_size= tb_md5_make (i_buffer, i_size, o_buffer, 16);
    if (o_size != 16) {
      return string ("");
    }

    tb_size_t i          = 0;
    tb_char_t md5_hex[32]= {0};
    for (i= 0; i < 16; ++i)
      tb_snprintf (md5_hex + (i << 1), 3, "%02x", o_buffer[i]);
    return string (md5_hex);
  }
  else {
    return string ("");
  }
}
} // namespace hash
} // namespace lolly
