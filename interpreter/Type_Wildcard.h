#pragma once

#include "Type_BaseBin.h"
#include "Wildcard.h"

/**
*/
class Type_Wildcard : public Type_BaseBin<Wildcard> {
public:
    Type_Wildcard();

    virtual Rc_string checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const override;
};

