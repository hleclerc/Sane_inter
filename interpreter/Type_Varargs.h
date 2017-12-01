#pragma once

#include "Type_BaseBin.h"
#include "Varargs.h"

/**
*/
class Type_Varargs : public Type_BaseBin<Varargs> {
public:
    Type_Varargs();

    virtual void spread_in      ( Scope *scope, const Variable &self, Vec<Variable> &res, Vec<Rc_string> &names ) override;
    virtual void write_to_stream( std::ostream &os, const char *content ) const;
};

