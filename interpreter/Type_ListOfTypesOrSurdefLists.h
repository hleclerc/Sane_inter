#pragma once

#include "ListOfTypesOrSurdefLists.h"
#include "Type_BaseBin.h"

/**
*/
class Type_ListOfTypesOrSurdefLists : public Type_BaseBin<ListOfTypesOrSurdefLists> {
public:
    Type_ListOfTypesOrSurdefLists();

    virtual Rc_string checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const override;
};

