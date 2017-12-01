#pragma once

#include "CallableWithSelfAndArgs.h"
#include "Type_BaseBin.h"

/**
*/
class Type_CallableWithSelfAndArgs : public Type_BaseBin<CallableWithSelfAndArgs> {
public:
    Type_CallableWithSelfAndArgs();

    virtual Variable apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true );
};

