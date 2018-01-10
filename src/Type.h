#pragma once

#include "Inst/KnownVal.h"
#include "TypeContent.h"
#include "ApplyFlags.h"
class Variable;
class Class;

/**
*/
class Type {
public:
    struct CondVal { int kv; Value val; }; ///< val is defined only if kv == 0. if kv < 0 => cond is false. if kv > 0 => cond is true
    Type( const LString &name );

    virtual bool          has_vtable_at_the_beginning() const;
    virtual RcString      checks_type_constraint     ( const Variable &self, const Variable &tested_var, TCI &tci ) const;
    virtual unsigned      get_nb_conversions         ( const Variable &self ) const;
    virtual bool          has_a_constructor          () const; ///< false for types like Union, ...
    virtual bool          primitive_number           () const;
    virtual void          write_to_stream            ( std::ostream &os ) const;
    virtual double        get_pertinence             ( const Variable &self ) const;
    virtual Variable      find_attribute             ( const RcString &name, const Variable &self, Variable::Flags flags, SI32 off ) const;
    virtual CondVal       get_condition              ( const Variable &self ) const;
    virtual void          get_fail_info              ( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const;
    virtual bool          destroy_attrs              () const;
    void                  add_attribute              ( const RcString &name, SI32 off, Type *type, Variable::Flags flags = Variable::Flags::NONE );
    virtual Variable      make_sl_trial              ( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) const;
    virtual Variable      use_sl_trial               ( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags, const Variable &trial ) const;
    Class                *orig_class                 () const;
    virtual void          write_cst                  ( std::ostream &os, const PI8 *data, int offset_mod_8 = 0, bool always_add_braces = false ) const;
    virtual void          spread_in                  ( const Variable &self, Vec<Variable> &res, Vec<RcString> &names );
    virtual void          construct                  ( const Variable &self, const Vec<Variable> &args, const Vec<RcString> &names );
    virtual Variable      with_self                  ( Variable &orig, const Variable &new_self ) const;
    virtual void          destroy                    ( const Variable &self, bool use_virtual );
    virtual String        c_name                     () const;
    virtual bool          error                      () const;
    virtual Variable      chbeba                     ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names );
    virtual Variable      select                     ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names );
    virtual Variable      apply                      ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags = ApplyFlags::NONE );

    //
    virtual int           is_signed                  () const;
    virtual int           mantissa_len               () const;
    virtual int           exponent_len               () const;

    KnownVal<TypeContent> content;
};

