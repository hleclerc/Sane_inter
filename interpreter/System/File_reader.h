#pragma once

#include <experimental/filesystem>
#include "Rc_string.h"

/**
  Read file content and add a \0 at the end.

  data is freed by the destructor
*/
struct File_reader {
    using path = std::experimental::filesystem::path;

    File_reader( const Rc_string &name );
    File_reader( const path &name );
    File_reader( const char *name );
    ~File_reader();

    operator bool() const;

    char  *data;
    size_t size;
};

