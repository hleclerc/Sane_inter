#pragma once

#include "TypeContent.h"
#include "ApplyFlags.h"
#include "KnownRef.h"
class Variable;
class Class;

/**
*/
class Type {
public:
    Type( const LString &name );

    virtual RcString      checks_type_constraint     ( const Variable &self, const Variable &tested_var, TCI &tci ) const;
    virtual unsigned      get_nb_conversions         ( const Variable &self ) const;
    virtual void          write_to_stream            ( std::ostream &os ) const;
    virtual double        get_pertinence             ( const Variable &self ) const;
    virtual bool          get_condition              ( const Variable &self ) const;
    virtual void          get_fail_info              ( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const;
    virtual bool          destroy_attrs              () const;
    virtual Variable      make_sl_trial              ( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) const;
    virtual Variable      use_sl_trial               ( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags, const Variable &trial ) const;
    virtual void          write_cst                  ( std::ostream &os, const PI8 *data, int offset_mod_8 = 0 ) const;
    virtual void          spread_in                  ( const Variable &self, Vec<Variable> &res, Vec<RcString> &names );
    virtual void          destroy                    ( const Variable &self, bool use_virtual );
    virtual Variable      apply                      ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags = ApplyFlags::NONE );

    virtual bool          error                      () const;

    virtual Variable      find_attribute             ( const RcString &name, const Variable &self, Variable::Flags flags, SI32 off ) const;
    virtual Variable      with_self                  ( Variable &orig, const Variable &new_self ) const;
    Class                *orig_class                 () const;

    KnownRef<TypeContent> content;
};

