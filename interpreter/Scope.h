#pragma once

#include "System/Error_list.h"
#include "System/EnumFlags.h"
#include "System/va_string.h"
#include "Ast/Ast_crepr.h"
#include "System/Deque.h"
#include "Variable.h"
#include <unordered_map>
#include <map>
#include <set>
class DelayedVarInScope;
class Import;
class Vm;

/**
*/
class Scope {
public:
    enum class VariableFlags : int { NONE = 0, CALLABLE = 1, STATIC = 2, EXPORT = 4, GLOBAL = 8, TEMPLATE = 16, CATCHED = 32, NO_DEC_REF = 64, SELF_AS_ARG = 128, SUPER = 256 }; ///< for each stored variable
    enum class Scope_type : int { ROOT, CALL, BLOCK, WHILE, TYPE_CTOR, FOR_BEG, FOR_EXE, WPC, TRY, CATCH, IF_EXE };
    struct NV { Rc_string name; Variable var; VariableFlags flags; NV *prev; };
    struct Position { Rc_string cur_names; size_t cur_src; size_t cur_off; };
    struct Exception { Variable var; Error_list::Error error; bool silent; };
    using MDV = std::map<Rc_string,Vec<DelayedVarInScope *>>;
    using MNV = std::map<Rc_string,NV>;

    Scope( Scope_type type, Scope *parent );
    Scope( Vm *vm );
    ~Scope();

    // error mgmt
    template<class ...Args>
    void                      add_error                ( const std::string &msg, const Args &...args ) const { return add_error( va_string( msg, args... ) ); }
    template<class ...Args>
    void                      add_error                ( int nb_calls_to_skip, const std::string &msg, const Args &...args ) const { return add_error( nb_calls_to_skip, va_string( msg, args... ) ); }
    void                      add_error                ( int nb_calls_to_skip, const std::string &msg ) const;
    void                      add_error                ( const std::string &msg ) const;

    template<class ...Args>
    void                      add_error_once           ( const Args &...args ) const { return add_error_once( va_string( args... ) ); }
    void                      add_error_once           ( const std::string &msg ) const;

    void                      disp_pos                 ( const std::string &msg ) const;

    Error_list::Error        &prep_error               ( const std::string &msg ) const;
    void                      disp_error               ( const Error_list::Error &error) const;

    Variable                 *add_static_variable      ( const Variable &var );
    size_t                    nb_scopes_to_catch       () const;
    size_t                    nb_scopes_to_break       () const;
    size_t                    nb_scopes_to_cont        () const;
    size_t                    nb_scopes_to_ret         () const;
    Scope                    *parent_for_vars          () { return type == Scope_type::CALL || type == Scope_type::FOR_EXE ? root : parent; }
    Scope                    *parent_interp            ( bool squeeze_for_beg = false ) const;
    Variable                  find_variable            ( const Rc_string &name, bool ret_err = true, bool allow_ambiant = true, bool ret_z_if_in_self = false );
    Exception                 exception                ( const Variable &var, bool silent = false );
    Variable                  find_self                ( bool ret_err = true );
    bool                      want_exec                () const { return nb_breaks == 0 && nb_conts == 0; }
    Rc_string                 src_name                 ( size_t index ) const;
    void                      reg_var                  ( const Rc_string &name, const Variable &var, VariableFlags flags = VariableFlags::NONE, bool check = true );
    void                      clear                    ();
    Variable                  visit                    ( const Vec<Ast_crepr> &creps, bool want_ret );
    Variable                  visit                    ( const Rc_string &names, const Vec<Rc_string> &code, bool want_ret );
    Variable                  visit                    ( const Rc_string &names, const Rc_string &code, bool want_ret );
    Variable                  visit                    ( const Ast_crepr &ac, bool want_ret );

    // std::set<ValidScopePtr *> valid_scope_ptrs;
    // Scope                    *valid_scope_ptr;  ///< for def, lambda, ... where some variable may not have been catched (e.g. smurf_${...})

    Vec<Rc_string>            __arguments_names;
    Rc_string                 self_method_name;
    Deque<NV>                 static_variables;
    Type                     *in_construction;
    MDV                       delayed_vars;
    std::set<Rc_string>       futur_attrs;
    MNV                       variables;
    Variable                  ctor_self;
    size_t                    nb_breaks;
    size_t                    nb_conts;
    NV                       *last_var;
    Scope                    *parent;
    Import                   *import;
    Variable                  self;
    Scope_type                type;
    Scope                    *root;
    Variable                  ret;
    std::set<Rc_string>      *wpc;
    Position                  pos;
    Vm                       *vm;
};
ENUM_FLAGS( Scope::VariableFlags )
