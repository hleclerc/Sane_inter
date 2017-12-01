#pragma once

#include "System/LVec.h"
#include "Variable.h"
#include "String.h"

/**
*/
struct Import {
    struct Export {
        void     write_to_stream( std::ostream &os ) const { os << name << "=" << var; }

        String   name;
        Variable var;
    };

    void         write_to_stream( std::ostream &os ) const { os << exports; }

    Rc_string    filename;
    LVec<Export> exports;
    bool         finished; ///< useful for recursive imports
};
