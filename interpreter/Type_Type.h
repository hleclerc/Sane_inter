#pragma once

#include "Type_BaseBin.h"
#include "Type.h"

/**
*/
class Type_Type : public Type_BaseBin<Type *> {
public:
    Type_Type();

    virtual Rc_string checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const override;
    virtual void      write_to_stream       ( std::ostream &os, const char *content ) const override;
    virtual bool      destroy_attrs         () const;
    static Variable   make_inst             ( Scope *scope, Type *type, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true );
    virtual Variable  apply                 ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true ) override;
};

