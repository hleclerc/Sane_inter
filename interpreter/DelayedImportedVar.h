#pragma once

#include "Import.h"

/**
  variable from an import. Used for recursive import, when some imports are not finished, preventing from getting the actual variables.
*/
struct DelayedImportedVar {
    void      write_to_stream( std::ostream &os ) const { os << "DelayedImportedVar(" << name << ")"; }

    Import   *import;
    Rc_string name;
};
