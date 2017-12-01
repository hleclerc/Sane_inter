#pragma once

#include "Type.h"

/**
*/
class Type_Union : public Type {
public:
    Type_Union( SI32 size, SI32 alig );

    virtual bool has_a_constructor() const override;
    virtual void construct        ( Scope *scope, const Variable &self, const Vec<Variable> &args, const Vec<Rc_string> &names ) override;
    virtual void destroy         ( Scope *scope, const Variable &self, bool use_virtual ) override;
};

