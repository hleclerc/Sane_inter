#pragma once

#include "FunctionSignature.h"
#include "System/Rc_string.h"
#include "System/Deque.h"
#include "Vtable.h"
#include <unordered_map>
#include <map>
#include <set>
class GetSetter;
class Scope;
class Class;
class TCI;

/**
*/
class Type {
public:
    struct Attribute { Rc_string name; SI32 off_in_bits; Type *type; Variable::Flags flags; Attribute *prev, *next; };
    using MSA = std::map<Rc_string,Variable *>;
    using MA  = std::map<Rc_string,Attribute>;
    using SFS = std::set<FunctionSignature>;

    Type( const Rc_string &name, PI32 size = 0, PI32 alig = 1 );
    virtual ~Type();

    bool                has_vtable_at_the_beginning() const;
    Type               *self_or_par_has_orig_class ( Class *cl );
    virtual Rc_string   checks_type_constraint     ( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const;
    virtual bool        base_arythmetic_type       () const;
    virtual unsigned    get_nb_conversions         ( Scope *scope, const Variable &self ) const;
    virtual bool        has_a_constructor          () const;
    void                apply_on_vtables           ( std::function<void( Type *type, SI32 off )> f, SI32 off = 0 );
    void                inheritance_rec            ( std::set<Type *> &res, bool start_with_parents = true );
    virtual void        write_to_stream            ( std::ostream &os, const char *content ) const;
    virtual void        write_to_stream            ( std::ostream &os ) const;
    void                disp_attributes            ( std::ostream &os );
    virtual Variable    find_attribute             ( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off );
    virtual FP64        get_pertinence             ( Scope *scope, const Variable &self ) const;
    virtual bool        get_condition              ( Scope *scope, const Variable &self ) const;
    virtual void        get_fail_info              ( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const;
    void                add_attribute              ( const Rc_string &name, SI32 off, Type *type, Variable::Flags flags = Variable::Flags::NONE );
    virtual Variable    make_sl_trial              ( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) const;
    virtual bool        destroy_attrs              () const;
    virtual Variable    use_sl_trial               ( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Variable &trial ) const;
    Class              *orig_class                 () const;
    virtual void        spread_in                  ( Scope *scope, const Variable &self, Vec<Variable> &res, Vec<Rc_string> &names );
    virtual Variable    with_self                  ( Scope *scope, const Variable &orig, const Variable &new_self );
    virtual bool        getsetter                  () const;
    virtual void        construct                  ( Scope *scope, const Variable &self, const Vec<Variable> &args, const Vec<Rc_string> &names );
    virtual void        destroy                    ( Scope *scope, const Variable &self, bool use_virtual = true );
    bool                inherits                   ( Scope *scope, Type *type );
    virtual void        bin_ctor                   ( Scope *scope, const Variable &self );
    virtual bool        is_false                   ( Scope *scope, const Variable &self );
    virtual bool        is_true                    ( Scope *scope, const Variable &self );
    virtual Variable    chbeba                     ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} );
    virtual Variable    select                     ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} );
    virtual Variable    apply                      ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true );
    virtual bool        error                      () const;


    MSA                 static_attributes;
    SFS                 abstract_methods;
    Attribute          *first_attribute;
    Attribute          *last_attribute;
    bool                has_new_vtable;
    Vec<Variable *>     parameters;        ///< template arguments
    MA                  attributes;
    MSA                 methods;
    Deque<Vtable>       vtables;
    Variable           *orig;              ///< orig Class
    Rc_string           name;
    SI32                size;              ///< < 0 means unknown (instance dependent)
    SI32                alig;              ///< < 0 means unknown (instance dependent)
};

