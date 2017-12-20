#pragma once

#include "Type.h"

/**
*/
class TypeVarargs : public Type {
public:
    TypeVarargs();

    virtual void spread_in( const Variable &self, Vec<Variable> &res, Vec<RcString> &names ) override;
};

