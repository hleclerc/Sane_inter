#pragma once

#include "AstVisitorBreakable.h"
#include "Variable.h"
class Scope;
class Vm;

/**
*/
class AstVisitorVm : public AstVisitorBreakable<Variable> {
public:
    AstVisitorVm( const RcString &names, bool want_ret );

    virtual void    set_src                  ( size_t src, size_t off ) override;

    virtual Variable on_variable             ( RcString name ) override;
    virtual Variable on_var_assembly         ( const Vec<RcString> &args ) override;
    virtual Variable on_break                () override;
    virtual Variable on_continue             () override;
    virtual Variable on_self                 () override;
    virtual Variable on_this                 () override;
    virtual Variable on_pass                 () override;
    virtual Variable on_false                () override;
    virtual Variable on_true                 () override;
    virtual Variable on_number               ( RcString value ) override;
    virtual Variable on_string               ( RcString value ) override;
    virtual Variable on_get_attr             ( RcString obj, RcString name ) override;
    virtual Variable on_get_attr_ptr         ( RcString obj, RcString name ) override;
    virtual Variable on_scope_resolution     ( RcString obj, RcString name ) override;
    virtual Variable on_get_attr_calc        ( RcString obj, const Vec<RcString> &args ) override;
    virtual Variable on_get_attr_ptr_calc    ( RcString obj, const Vec<RcString> &args ) override;
    virtual Variable on_scope_resolution_calc( RcString obj, const Vec<RcString> &args ) override;
    virtual Variable on_block                ( const Vec<RcString> &items ) override;
    virtual Variable on_apply_op             ( RcString f, const Vec<RcString> &args ) override;
    virtual Variable on_apply                ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) override;
    virtual Variable on_select               ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) override;
    virtual Variable on_chbeba               ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) override;
    virtual Variable on_init_of              ( RcString name, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) override;
    virtual Variable on_assign               ( RcString name, RcString cname, PI8 nb_scopes_res, RcString value, PI8 flags ) override;
    virtual Variable on_ss_block             ( const Vec<RcString> &items );
    virtual Variable on_destruct_assign      ( const Vec<RcString> &names, const Vec<RcString> &equal, const Vec<RcString> &types, RcString value, PI8 flags ) override;
    virtual Variable on_def                  ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, RcString self_constraint, RcString return_type, const Vec<RcString> &wpc ) override;
    virtual Variable on_class                ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, const Vec<RcString> &inheritance_names, const Vec<RcString> &inheritance ) override;
    virtual Variable on_if                   ( RcString cond, const Vec<RcString> &ok ) override;
    virtual Variable on_if_else              ( RcString cond, const Vec<RcString> &ok, const Vec<RcString> &ko ) override;
    virtual Variable on_while                ( RcString cond, const Vec<RcString> &ok ) override;
    virtual Variable on_return               ( RcString value ) override;
    virtual Variable on_for                  ( RcString name, RcString container, const Vec<RcString> &block ) override;
    virtual Variable on_typeof               ( RcString value ) override;
    virtual Variable on_sizeof               ( RcString value ) override;
    virtual Variable on_aligof               ( RcString value ) override;
    virtual Variable on_sizeof_bits          ( RcString value ) override;
    virtual Variable on_aligof_bits          ( RcString value ) override;
    virtual Variable on_lambda               ( const Vec<RcString> &names, const Vec<RcString> &types, const Vec<RcString> &def_vals, const Vec<size_t> &spreads, RcString body ) override;
    virtual Variable on_and                  ( RcString v0, RcString v1 ) override;
    virtual Variable on_or                   ( RcString v0, RcString v1 ) override;
    virtual Variable on_vec                  ( const Vec<RcString> &values ) override;
    virtual Variable on_map                  ( const Vec<RcString> &names, const Vec<RcString> &values ) override;
    virtual Variable on_matrix               ( const Vec<RcString> &values, const Vec<size_t> &sizes ) override;
    virtual Variable on_raii                 ( const Vec<RcString> &code ) override;
    virtual Variable on_info                 ( const Vec<RcString> &str, const Vec<RcString> &code ) override;
    virtual Variable on_assert               ( RcString str, RcString code ) override;
    virtual Variable on_try_catch            ( RcString try_body, RcString arg_name, RcString arg_constraint, RcString catch_body, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_splats ) override;
    virtual Variable on_import               ( RcString filename, const Vec<RcString> &names, const Vec<RcString> &args ) override;
    virtual Variable on_enum                 ( RcString name, const Vec<RcString> &items ) override;

    virtual void     default_enter           ( const char *name );

    static void      init_of                 ( Scope *scope, RcString name, const Vec<Variable> &args, const Vec<RcString> &names = {}, const Vec<size_t> &spreads = {} );

    Variable         xxxxof                  ( RcString value, int w, bool in_bytes = false ); ///< 0 -> typeof, 1 -> sizeof, 2 -> aligof
    static Variable  assign                  ( Scope *scope, RcString name, std::function<Variable()> rhs_func, PI8 flags );
    void             ret_or_dec_ref          ( Variable &&var );
    void             ret_or_dec_ref          ( Variable &var );
    RcString         string_assembly         ( const Vec<RcString> &args );

    RcString         names;
    bool             want_ret;
};
