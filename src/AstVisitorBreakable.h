#pragma once

#include "Ast/AstVisitor.h"
#include "gvm.h"

/**
*/
template<class T>
class AstVisitorBreakable : public AstVisitor {
public:
    void assign( const T &val ) { ret = val; }

    virtual bool enter_test_variable             ( RcString name )                                                                                     override { if ( gvm->want_exec() ) assign( on_variable             ( name                                     ) ); return false; }
    virtual bool enter_test_var_assembly         ( const Vec<RcString> &args )                                                                         override { if ( gvm->want_exec() ) assign( on_var_assembly         ( args                                     ) ); return false; }
    virtual bool enter_test_break                ()                                                                                                    override { if ( gvm->want_exec() ) assign( on_break                (                                          ) ); return false; }
    virtual bool enter_test_continue             ()                                                                                                    override { if ( gvm->want_exec() ) assign( on_continue             (                                          ) ); return false; }
    virtual bool enter_test_self                 ()                                                                                                    override { if ( gvm->want_exec() ) assign( on_self                 (                                          ) ); return false; }
    virtual bool enter_test_this                 ()                                                                                                    override { if ( gvm->want_exec() ) assign( on_this                 (                                          ) ); return false; }
    virtual bool enter_test_pass                 ()                                                                                                    override { if ( gvm->want_exec() ) assign( on_pass                 (                                          ) ); return false; }
    virtual bool enter_test_false                ()                                                                                                    override { if ( gvm->want_exec() ) assign( on_false                (                                          ) ); return false; }
    virtual bool enter_test_true                 ()                                                                                                    override { if ( gvm->want_exec() ) assign( on_true                 (                                          ) ); return false; }
    virtual bool enter_test_number               ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_number               ( value                                    ) ); return false; }
    virtual bool enter_test_string               ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_string               ( value                                    ) ); return false; }
    virtual bool enter_test_get_attr             ( RcString obj, RcString name )                                                                       override { if ( gvm->want_exec() ) assign( on_get_attr             ( obj, name                                ) ); return false; }
    virtual bool enter_test_get_attr_ptr         ( RcString obj, RcString name )                                                                       override { if ( gvm->want_exec() ) assign( on_get_attr_ptr         ( obj, name                                ) ); return false; }
    virtual bool enter_test_scope_resolution     ( RcString obj, RcString name )                                                                       override { if ( gvm->want_exec() ) assign( on_scope_resolution     ( obj, name                                ) ); return false; }
    virtual bool enter_test_get_attr_calc        ( RcString obj, const Vec<RcString> &args )                                                           override { if ( gvm->want_exec() ) assign( on_get_attr_calc        ( obj, args                                ) ); return false; }
    virtual bool enter_test_get_attr_ptr_calc    ( RcString obj, const Vec<RcString> &args )                                                           override { if ( gvm->want_exec() ) assign( on_get_attr_ptr_calc    ( obj, args                                ) ); return false; }
    virtual bool enter_test_scope_resolution_calc( RcString obj, const Vec<RcString> &args )                                                           override { if ( gvm->want_exec() ) assign( on_scope_resolution_calc( obj, args                                ) ); return false; }
    virtual bool enter_test_block                ( const Vec<RcString> &items )                                                                        override { if ( gvm->want_exec() ) assign( on_block                ( items                                    ) ); return false; }
    virtual bool enter_test_apply_op             ( RcString f, const Vec<RcString> &args )                                                             override { if ( gvm->want_exec() ) assign( on_apply_op             ( f, args                                  ) ); return false; }
    virtual bool enter_test_apply                ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads )     override { if ( gvm->want_exec() ) assign( on_apply                ( f, args, names, spreads                  ) ); return false; }
    virtual bool enter_test_select               ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads )     override { if ( gvm->want_exec() ) assign( on_select               ( f, args, names, spreads                  ) ); return false; }
    virtual bool enter_test_chbeba               ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads )     override { if ( gvm->want_exec() ) assign( on_chbeba               ( f, args, names, spreads                  ) ); return false; }
    virtual bool enter_test_init_of              ( RcString name, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads )  override { if ( gvm->want_exec() ) assign( on_init_of              ( name, args, names, spreads               ) ); return false; }
    virtual bool enter_test_assign               ( RcString name, RcString cname, PI8 nb_scopes_res, RcString value, PI8 flags )                       override { if ( gvm->want_exec() ) assign( on_assign               ( name, cname, nb_scopes_res, value, flags ) ); return false; }
    virtual bool enter_test_ss_block             ( const Vec<RcString> &items )                                                                        override { if ( gvm->want_exec() ) assign( on_ss_block             ( items                                    ) ); return false; }
    virtual bool enter_test_destruct_assign      ( const Vec<RcString> &names, const Vec<RcString> &equal, const Vec<RcString> &types,
                                                   RcString value, PI8 flags )                                                                         override { if ( gvm->want_exec() ) assign( on_destruct_assign      ( names, equal, types, value, flags        ) ); return false; }
    virtual bool enter_test_def                  ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints,
                                                   const Vec<RcString> &arg_def_vals,  const Vec<size_t> &arg_spreads,
                                                   const Vec<RcString> &with_names, const Vec<RcString> &with_constraints,
                                                   const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition,
                                                   RcString pertinence,  SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block,
                                                   RcString self_constraint, RcString return_type, const Vec<RcString> &wpc )                          override { if ( gvm->want_exec() ) assign( on_def                  ( name, cname, nb_scopes_rec, arg_names, arg_constraints, arg_def_vals, arg_spreads, with_names, with_constraints, with_def_vals, with_spreads, condition, pertinence, def_pert_num, def_pert_exp, flags, block, self_constraint, return_type, wpc ) ); return false; }
    virtual bool enter_test_class                ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints,
                                                   const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads,
                                                   const Vec<RcString> &with_names, const Vec<RcString> &with_constraints,
                                                   const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition,
                                                   RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block,
                                                   const Vec<RcString> &inheritance_names, const Vec<RcString> &inheritance )                          override { if ( gvm->want_exec() ) assign( on_class                ( name, cname, nb_scopes_rec, arg_names, arg_constraints, arg_def_vals, arg_spreads, with_names, with_constraints, with_def_vals, with_spreads, condition, pertinence, def_pert_num, def_pert_exp, flags, block, inheritance_names, inheritance ) ); return false; }
    virtual bool enter_test_if                   ( RcString cond, const Vec<RcString> &ok )                                                            override { if ( gvm->want_exec() ) assign( on_if                   ( cond, ok               ) ); return false; }
    virtual bool enter_test_if_else              ( RcString cond, const Vec<RcString> &ok, const Vec<RcString> &ko )                                   override { if ( gvm->want_exec() ) assign( on_if_else              ( cond, ok, ko           ) ); return false; }
    virtual bool enter_test_while                ( RcString cond, const Vec<RcString> &ok )                                                            override { if ( gvm->want_exec() ) assign( on_while                ( cond, ok               ) ); return false; }
    virtual bool enter_test_return               ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_return               ( value                  ) ); return false; }
    virtual bool enter_test_for                  ( RcString name, RcString container, const Vec<RcString> &block )                                     override { if ( gvm->want_exec() ) assign( on_for                  ( name, container, block ) ); return false; }
    virtual bool enter_test_typeof               ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_typeof               ( value                  ) ); return false; }
    virtual bool enter_test_sizeof               ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_sizeof               ( value                  ) ); return false; }
    virtual bool enter_test_aligof               ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_aligof               ( value                  ) ); return false; }
    virtual bool enter_test_sizeof_bits          ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_sizeof_bits          ( value                  ) ); return false; }
    virtual bool enter_test_aligof_bits          ( RcString value )                                                                                    override { if ( gvm->want_exec() ) assign( on_aligof_bits          ( value                  ) ); return false; }
    virtual bool enter_test_lambda               ( const Vec<RcString> &names, const Vec<RcString> &types, const Vec<RcString> &def_vals,
                                                   const Vec<size_t> &spreads, RcString body )                                                         override { if ( gvm->want_exec() ) assign( on_lambda               ( names, types, def_vals, spreads, body ) ); return false; }
    virtual bool enter_test_and                  ( RcString v0, RcString v1 )                                                                          override { if ( gvm->want_exec() ) assign( on_and                  ( v0, v1                 ) ); return false; }
    virtual bool enter_test_or                   ( RcString v0, RcString v1 )                                                                          override { if ( gvm->want_exec() ) assign( on_or                   ( v0, v1                 ) ); return false; }
    virtual bool enter_test_vec                  ( const Vec<RcString> &values )                                                                       override { if ( gvm->want_exec() ) assign( on_vec                  ( values                 ) ); return false; }
    virtual bool enter_test_map                  ( const Vec<RcString> &names, const Vec<RcString> &values )                                           override { if ( gvm->want_exec() ) assign( on_map                  ( names, values          ) ); return false; }
    virtual bool enter_test_matrix               ( const Vec<RcString> &values, const Vec<size_t> &sizes )                                             override { if ( gvm->want_exec() ) assign( on_matrix               ( values, sizes          ) ); return false; }
    virtual bool enter_test_raii                 ( const Vec<RcString> &code )                                                                         override { if ( gvm->want_exec() ) assign( on_raii                 ( code                   ) ); return false; }
    virtual bool enter_test_info                 ( const Vec<RcString> &str, const Vec<RcString> &code )                                               override { if ( gvm->want_exec() ) assign( on_info                 ( str, code              ) ); return false; }
    virtual bool enter_test_assert               ( RcString str, RcString code )                                                                       override { if ( gvm->want_exec() ) assign( on_assert               ( str, code              ) ); return false; }
    virtual bool enter_test_try_catch            ( RcString try_body, RcString arg_name, RcString arg_constraint, RcString catch_body, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_splats ) override { if ( gvm->want_exec() ) assign( on_try_catch( try_body, arg_name, arg_constraint, catch_body, with_names, with_constraints, with_def_vals, with_splats ) ); return false; }
    virtual bool enter_test_import               ( RcString filename, const Vec<RcString> &names, const Vec<RcString> &args )                          override { if ( gvm->want_exec() ) assign( on_import               ( filename, names, args  ) ); return false; }
    virtual bool enter_test_enum                 ( RcString name, const Vec<RcString> &items )                                                         override { if ( gvm->want_exec() ) assign( on_enum                 ( name, items            ) ); return false; }

    virtual T    on_variable                     ( RcString name ) = 0;
    virtual T    on_var_assembly                 ( const Vec<RcString> &args ) = 0;
    virtual T    on_break                        () = 0;
    virtual T    on_continue                     () = 0;
    virtual T    on_self                         () = 0;
    virtual T    on_this                         () = 0;
    virtual T    on_pass                         () = 0;
    virtual T    on_false                        () = 0;
    virtual T    on_true                         () = 0;
    virtual T    on_number                       ( RcString value ) = 0;
    virtual T    on_string                       ( RcString value ) = 0;
    virtual T    on_get_attr                     ( RcString obj, RcString name ) = 0;
    virtual T    on_get_attr_ptr                 ( RcString obj, RcString name ) = 0;
    virtual T    on_scope_resolution             ( RcString obj, RcString name ) = 0;
    virtual T    on_get_attr_calc                ( RcString obj, const Vec<RcString> &args ) = 0;
    virtual T    on_get_attr_ptr_calc            ( RcString obj, const Vec<RcString> &args ) = 0;
    virtual T    on_scope_resolution_calc        ( RcString obj, const Vec<RcString> &args ) = 0;
    virtual T    on_block                        ( const Vec<RcString> &items ) = 0;
    virtual T    on_apply_op                     ( RcString f, const Vec<RcString> &args ) = 0;
    virtual T    on_apply                        ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) = 0;
    virtual T    on_select                       ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) = 0;
    virtual T    on_chbeba                       ( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) = 0;
    virtual T    on_init_of                      ( RcString name, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) = 0;
    virtual T    on_assign                       ( RcString name, RcString cname, PI8 nb_scopes_res, RcString value, PI8 flags ) = 0;
    virtual T    on_ss_block                     ( const Vec<RcString> &items ) = 0;
    virtual T    on_destruct_assign              ( const Vec<RcString> &names, const Vec<RcString> &equal, const Vec<RcString> &types, RcString value, PI8 flags ) = 0;
    virtual T    on_def                          ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, RcString self_constraint, RcString return_type, const Vec<RcString> &wpc ) = 0;
    virtual T    on_class                        ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, const Vec<RcString> &inheritance_names, const Vec<RcString> &inheritance ) = 0;
    virtual T    on_if                           ( RcString cond, const Vec<RcString> &ok ) = 0;
    virtual T    on_if_else                      ( RcString cond, const Vec<RcString> &ok, const Vec<RcString> &ko ) = 0;
    virtual T    on_while                        ( RcString cond, const Vec<RcString> &ok ) = 0;
    virtual T    on_return                       ( RcString value ) = 0;
    virtual T    on_for                          ( RcString name, RcString container, const Vec<RcString> &block ) = 0;
    virtual T    on_typeof                       ( RcString value ) = 0;
    virtual T    on_sizeof                       ( RcString value ) = 0;
    virtual T    on_aligof                       ( RcString value ) = 0;
    virtual T    on_sizeof_bits                  ( RcString value ) = 0;
    virtual T    on_aligof_bits                  ( RcString value ) = 0;
    virtual T    on_lambda                       ( const Vec<RcString> &names, const Vec<RcString> &types, const Vec<RcString> &def_vals, const Vec<size_t> &spreads, RcString body ) = 0;
    virtual T    on_and                          ( RcString v0, RcString v1 ) = 0;
    virtual T    on_or                           ( RcString v0, RcString v1 ) = 0;
    virtual T    on_vec                          ( const Vec<RcString> &values ) = 0;
    virtual T    on_map                          ( const Vec<RcString> &names, const Vec<RcString> &values ) = 0;
    virtual T    on_matrix                       ( const Vec<RcString> &values, const Vec<size_t> &sizes ) = 0;
    virtual T    on_raii                         ( const Vec<RcString> &code ) = 0;
    virtual T    on_info                         ( const Vec<RcString> &str, const Vec<RcString> &code ) = 0;
    virtual T    on_assert                       ( RcString str, RcString code ) = 0;
    virtual T    on_try_catch                    ( RcString try_body, RcString arg_name, RcString arg_constraint, RcString catch_body, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_splats ) = 0;
    virtual T    on_import                       ( RcString filename, const Vec<RcString> &names, const Vec<RcString> &args ) = 0;
    virtual T    on_enum                         ( RcString name, const Vec<RcString> &items ) = 0;

    T            ret;
};

