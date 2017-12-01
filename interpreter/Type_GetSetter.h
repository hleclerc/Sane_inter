#pragma once

#include "Type_BaseBin.h"
#include "GetSetter.h"

/**
*/
class Type_GetSetter : public Type_BaseBin<GetSetter> {
public:
    Type_GetSetter();

    virtual Rc_string checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const;
    virtual Variable  find_attribute        ( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off );
    virtual FP64      get_pertinence        ( Scope *scope, const Variable &self ) const;
    virtual bool      get_condition         ( Scope *scope, const Variable &self ) const;
    virtual void      get_fail_info         ( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const;
    virtual Variable  make_sl_trial         ( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) const;
    virtual Variable  use_sl_trial          ( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Variable &trial ) const;
    virtual void      spread_in             ( Scope *scope, const Variable &self, Vec<Variable> &res, Vec<Rc_string> &names );
    virtual Variable  with_self             ( Scope *scope, const Variable &orig, const Variable &new_self );
    virtual bool      getsetter             () const override;
    virtual bool      is_false              ( Scope *scope, const Variable &self ) override;
    virtual bool      is_true               ( Scope *scope, const Variable &self ) override;
    virtual Variable  chbeba                ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} ) override;
    virtual Variable  select                ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} ) override;
    virtual Variable  apply                 ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true ) override;
};
