#pragma once

#include "Type_BaseBin.h"
#include "Import.h"

/**
*/
class Type_Import : public Type_BaseBin<Import> {
public:
    Type_Import();

    virtual Variable find_attribute( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off );
};

