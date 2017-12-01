#pragma once

#include "DelayedVarInScope.h"
#include "Type_BaseBin.h"

/**
*/
class Type_DelayedVarInScope : public Type_BaseBin<DelayedVarInScope> {
public:
    Type_DelayedVarInScope();

    virtual Variable apply ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true ) override;
};

