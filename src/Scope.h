#pragma once

#include "System/RcString.h"
#include "System/Deque.h"
#include "Variable.h"
#include <map>
#include <set>
class DelayedVarInScope;
class AstCrepr;
class Import;

/**
*/
class Scope {
public:
    enum class VariableFlags : int { NONE = 0, CALLABLE = 1, STATIC = 2, EXPORT = 4, GLOBAL = 8, TEMPLATE = 16, CATCHED = 32, NO_DEC_REF = 64, SELF_AS_ARG = 128, SUPER = 256 }; ///< for each stored variable
    enum class ScopeType : int { ROOT, CALL, BLOCK, WHILE, TYPE_CTOR, FOR_BEG, FOR_EXE, WPC, TRY, CATCH, IF_EXE };
    struct NV { RcString name; Variable var; VariableFlags flags; NV *prev; };
    using MDV = std::map<RcString,Vec<DelayedVarInScope *>>;
    using MNV = std::map<RcString,NV>;

    Scope( ScopeType type );
    ~Scope();

    Variable            find_variable            ( const RcString &name, bool ret_err = true, bool allow_ambiant = true, bool ret_z_if_in_self = false );
    Variable            find_self                ( bool ret_err = true );
    void                reg_var                  ( const RcString &name, const Variable &var, VariableFlags flags = VariableFlags::NONE, bool check = true );

    size_t              nb_scopes_to_catch       () const;
    size_t              nb_scopes_to_break       () const;
    size_t              nb_scopes_to_cont        () const;
    size_t              nb_scopes_to_ret         () const;

    Scope              *parent_interp            ( bool squeeze_for_beg = false ) const;
    Scope              *parent_for_vars          () { return type == ScopeType::CALL || type == ScopeType::FOR_EXE ? root : parent; }
    Variable           *add_static_variable      ( const Variable &var );


    Vec<RcString>       __arguments_names;
    RcString            self_method_name;
    Deque<NV>           static_variables;
    Type               *in_construction;
    MDV                 delayed_vars;
    std::set<RcString>  futur_attrs;
    Variable            ctor_self;
    MNV                 variables;
    NV                 *last_var;
    Scope              *parent;
    Import             *import;
    Variable            self;
    Scope              *root;
    ScopeType           type;
    Variable            ret;
    std::set<RcString> *wpc;
};
ENUM_FLAGS( Scope::VariableFlags )
