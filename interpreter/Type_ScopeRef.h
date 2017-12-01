#pragma once

#include "Type_BaseBin.h"

/**
*/
class Type_ScopeRef : public Type_BaseBin<Scope *> {
public:
    Type_ScopeRef();

    virtual Variable find_attribute( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off );
};

