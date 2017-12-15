#pragma once

#include "System/LString.h"
#include "System/LVec.h"
#include "Variable.h"

/**
*/
struct Import {
    struct Export {
        void     write_to_stream( std::ostream &os ) const { os << name << "=" << var; }

        LString  name;
        Variable var;
    };

    void         write_to_stream( std::ostream &os ) const { os << exports; }

    LString      filename;
    LVec<Export> exports;
    bool         finished; ///< useful for recursive imports
};
