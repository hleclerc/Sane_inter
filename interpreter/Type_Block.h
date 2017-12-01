#pragma once

#include "Type_BaseBin.h"
#include "Block.h"

/**
*/
class Type_Block : public Type_BaseBin<Block> {
public:
    Type_Block();

    virtual Variable apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true );
};

