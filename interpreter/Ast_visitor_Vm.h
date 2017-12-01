#pragma once

#include "AstVisitorBreakable.h"
#include "Variable.h"
class Scope;
class Vm;

/**
*/
class Ast_visitor_Vm : public AstVisitorBreakable {
public:
    Ast_visitor_Vm( Scope *scope, const Rc_string &names, bool want_ret );

    virtual void    set_src                 ( size_t src, size_t off ) override;

    virtual void    on_variable             ( Rc_string name ) override;
    virtual void    on_var_assembly         ( const Vec<Rc_string> &args ) override;
    virtual void    on_break                () override;
    virtual void    on_continue             () override;
    virtual void    on_self                 () override;
    virtual void    on_this                 () override;
    virtual void    on_pass                 () override;
    virtual void    on_false                () override;
    virtual void    on_true                 () override;
    virtual void    on_number               ( Rc_string value ) override;
    virtual void    on_string               ( Rc_string value ) override;
    virtual void    on_get_attr             ( Rc_string obj, Rc_string name ) override;
    virtual void    on_get_attr_ptr         ( Rc_string obj, Rc_string name ) override;
    virtual void    on_scope_resolution     ( Rc_string obj, Rc_string name ) override;
    virtual void    on_get_attr_calc        ( Rc_string obj, const Vec<Rc_string> &args ) override;
    virtual void    on_get_attr_ptr_calc    ( Rc_string obj, const Vec<Rc_string> &args ) override;
    virtual void    on_scope_resolution_calc( Rc_string obj, const Vec<Rc_string> &args ) override;
    virtual void    on_block                ( const Vec<Rc_string> &items ) override;
    virtual void    on_apply_op             ( Rc_string f, const Vec<Rc_string> &args ) override;
    virtual void    on_apply                ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) override;
    virtual void    on_select               ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) override;
    virtual void    on_chbeba               ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) override;
    virtual void    on_init_of              ( Rc_string name, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) override;
    virtual void    on_assign               ( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags ) override;
    virtual void    on_ss_block             ( const Vec<Rc_string> &items );
    virtual void    on_destruct_assign      ( const Vec<Rc_string> &names, const Vec<Rc_string> &equal, const Vec<Rc_string> &types, Rc_string value, PI8 flags ) override;
    virtual void    on_def                  ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc ) override;
    virtual void    on_class                ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance ) override;
    virtual void    on_if                   ( Rc_string cond, const Vec<Rc_string> &ok ) override;
    virtual void    on_if_else              ( Rc_string cond, const Vec<Rc_string> &ok, const Vec<Rc_string> &ko ) override;
    virtual void    on_while                ( Rc_string cond, const Vec<Rc_string> &ok ) override;
    virtual void    on_return               ( Rc_string value ) override;
    virtual void    on_for                  ( Rc_string name, Rc_string container, const Vec<Rc_string> &block ) override;
    virtual void    on_typeof               ( Rc_string value ) override;
    virtual void    on_sizeof               ( Rc_string value ) override;
    virtual void    on_aligof               ( Rc_string value ) override;
    virtual void    on_sizeof_bits          ( Rc_string value ) override;
    virtual void    on_aligof_bits          ( Rc_string value ) override;
    virtual void    on_lambda               ( const Vec<Rc_string> &names, const Vec<Rc_string> &types, const Vec<Rc_string> &def_vals, const Vec<size_t> &spreads, Rc_string body ) override;
    virtual void    on_and                  ( Rc_string v0, Rc_string v1 ) override;
    virtual void    on_or                   ( Rc_string v0, Rc_string v1 ) override;
    virtual void    on_vec                  ( const Vec<Rc_string> &values ) override;
    virtual void    on_map                  ( const Vec<Rc_string> &names, const Vec<Rc_string> &values ) override;
    virtual void    on_matrix               ( const Vec<Rc_string> &values, const Vec<size_t> &sizes ) override;
    virtual void    on_raii                 ( const Vec<Rc_string> &code ) override;
    virtual void    on_info                 ( const Vec<Rc_string> &str, const Vec<Rc_string> &code ) override;
    virtual void    on_assert               ( Rc_string str, Rc_string code ) override;
    virtual void    on_try_catch            ( Rc_string try_body, Rc_string arg_name, Rc_string arg_constraint, Rc_string catch_body, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_splats ) override;
    virtual void    on_import               ( Rc_string filename, const Vec<Rc_string> &names, const Vec<Rc_string> &args ) override;
    virtual void    on_enum                 ( Rc_string name, const Vec<Rc_string> &items ) override;

    virtual void    default_enter           ( const char *name );

    static void     init_of                 ( Scope *scope, Rc_string name, const Vec<Variable> &args, const Vec<Rc_string> &names = {}, const Vec<size_t> &spreads = {} );

    Variable        xxxxof                  ( Rc_string value, int w, bool in_bytes = false ); ///< 0 -> typeof, 1 -> sizeof, 2 -> aligof
    static Variable assign                  ( Scope *scope, Rc_string name, std::function<Variable()> rhs_func, PI8 flags );
    void            ret_or_dec_ref          ( Variable &&var );
    void            ret_or_dec_ref          ( Variable &var );
    Rc_string       string_assembly         ( const Vec<Rc_string> &args );

    Rc_string       names;
    Variable        ret;      ///< returned variable
    Vm             *vm;
    bool            want_ret;
};
