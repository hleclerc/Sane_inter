#pragma once

#include "Type_BaseBin.h"
#include "SurdefList.h"

/**
*/
class Type_SurdefList : public Type_BaseBin<SurdefList> {
public:
    Type_SurdefList();

    virtual Rc_string checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const override;
    virtual void      write_to_stream       ( std::ostream &os, const char *content ) const override;
    virtual Variable  with_self             ( Scope *scope, const Variable &orig, const Variable &new_self ) override;
    virtual Variable  select                ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} ) override;
    virtual Variable  apply                 ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true ) override;
};

