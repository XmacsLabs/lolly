#include "file.hpp"
#include "string.hpp"
#include "tbox/tbox.h"

bool
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
    TM_FAILED ("is_directory failed")
  }
}

bool
is_directory (file_url name) {
  string path= name.concretize ();
  return is_directory (path);
}