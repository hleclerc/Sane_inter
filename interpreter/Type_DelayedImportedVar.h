#pragma once

#include "Type_BaseBin.h"
#include "DelayedImportedVar.h"

/**
*/
class Type_DelayedImportedVar : public Type_BaseBin<DelayedImportedVar> {
public:
    Type_DelayedImportedVar();

    virtual Variable apply ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true ) override;
};

