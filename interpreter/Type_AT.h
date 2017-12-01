#pragma once

#include "Type_BaseBin.h"
#include "AT.h"

/**
*/
class Type_AT : public Type_BaseBin<AT> {
public:
    Type_AT();

    virtual bool is_false( Scope *scope, const Variable &self );
    virtual bool is_true ( Scope *scope, const Variable &self );
};

