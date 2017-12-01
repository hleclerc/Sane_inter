#pragma once

#include "Type.h"

/**
*/
class Type_Error : public Type {
public:
    Type_Error();

    virtual void      write_to_stream( std::ostream &os, const char *content ) const override;
    virtual Variable  make_sl_trial  ( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) const;
    virtual Variable  use_sl_trial   ( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Variable &trial ) const;
    virtual void      spread_in      ( Scope *scope, const Vec<Variable> &res );
    virtual Variable  with_self      ( Scope *scope, const Variable &orig, const Variable &new_self );
    virtual void      destroy       ( Scope *scope, const Variable &self, bool use_virtual ) override;
    virtual Variable  chbeba         ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} );
    virtual Variable  select         ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} );
    virtual Variable  apply          ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true );
    virtual bool      error          () const override;
};

