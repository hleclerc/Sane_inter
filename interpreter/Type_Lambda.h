#pragma once

#include "Type_BaseBin.h"
#include "Lambda.h"

/**
*/
class Type_Lambda : public Type_BaseBin<Lambda> {
public:
    Type_Lambda();

    virtual bool     destroy_attrs() const override;
    virtual Variable with_self    ( Scope *scope, const Variable &orig, const Variable &new_self ) override;
    virtual Variable chbeba       ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} ) override;
    virtual Variable select       ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} ) override;
    virtual Variable apply        ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true ) override;
};

