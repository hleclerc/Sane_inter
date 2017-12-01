#pragma once

#include "Type.h"
#include "TCI.h"

/**
*/
struct SlTrialClass {
    void          write_to_stream( std::ostream &os ) const { os << "SlTrialClass(...)"; }

    Vec<Variable> args;
    Variable      def;
    Rc_string     msg;
    TCI           tci;
    Bool          condition;
};
