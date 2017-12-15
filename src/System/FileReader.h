#pragma once

#include <experimental/filesystem>

/**
  Read file content and add a \0 at the end.

  data is freed by the destructor
*/
struct FileReader {
    using path = std::experimental::filesystem::path;

    FileReader( const path &name );
    ~FileReader();

    operator bool() const;

    char  *data;
    size_t size;
};

