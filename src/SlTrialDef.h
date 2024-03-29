#pragma once

#include "Type.h"
#include "TCI.h"

/**
*/
struct SlTrialDef {
    void          write_to_stream( std::ostream &os ) const { os << "SlTrialDef(...)"; }

    Vec<Variable> args;
    Variable      def;
    RcString      msg;
    TCI           tci;
    Type::CondVal condition;
};
