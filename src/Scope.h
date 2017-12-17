#pragma once

#include "System/RcString.h"
#include "Variable.h"
class AstCrepr;
class Import;

/**
*/
class Scope {
public:
    enum class ScopeType : int { ROOT, CALL, BLOCK, WHILE, TYPE_CTOR, FOR_BEG, FOR_EXE, WPC, TRY, CATCH, IF_EXE };

    Scope( ScopeType type );
    ~Scope();

    Variable   find_variable            ( const Rc_string &name, bool ret_err = true, bool allow_ambiant = true, bool ret_z_if_in_self = false );

    size_t     nb_scopes_to_catch       () const;
    size_t     nb_scopes_to_break       () const;
    size_t     nb_scopes_to_cont        () const;
    size_t     nb_scopes_to_ret         () const;

    Scope     *parent_interp            ( bool squeeze_for_beg = false ) const;
    Scope     *parent_for_vars          () { return type == ScopeType::CALL || type == ScopeType::FOR_EXE ? root : parent; }

    Scope     *parent;
    Import    *import;
    Scope     *root;
    ScopeType type;
};

