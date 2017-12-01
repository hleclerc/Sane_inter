#pragma once

#include "Ast/Ast_visitor.h"
#include "Scope.h"

/**
*/
class AstVisitorBreakable : public AstVisitor {
public:
    AstVisitorBreakable( Scope *scope );
    ~AstVisitorBreakable();

    virtual bool enter_test_variable             ( Rc_string name )                                                                                      override { if ( scope->want_exec() ) on_variable             ( name ); return false; }
    virtual bool enter_test_var_assembly         ( const Vec<Rc_string> &args )                                                                          override { if ( scope->want_exec() ) on_var_assembly         ( args ); return false; }
    virtual bool enter_test_break                ()                                                                                                      override { if ( scope->want_exec() ) on_break                (); return false; }
    virtual bool enter_test_continue             ()                                                                                                      override { if ( scope->want_exec() ) on_continue             (); return false; }
    virtual bool enter_test_self                 ()                                                                                                      override { if ( scope->want_exec() ) on_self                 (); return false; }
    virtual bool enter_test_this                 ()                                                                                                      override { if ( scope->want_exec() ) on_this                 (); return false; }
    virtual bool enter_test_pass                 ()                                                                                                      override { if ( scope->want_exec() ) on_pass                 (); return false; }
    virtual bool enter_test_false                ()                                                                                                      override { if ( scope->want_exec() ) on_false                (); return false; }
    virtual bool enter_test_true                 ()                                                                                                      override { if ( scope->want_exec() ) on_true                 (); return false; }
    virtual bool enter_test_number               ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_number               ( value ); return false; }
    virtual bool enter_test_string               ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_string               ( value ); return false; }
    virtual bool enter_test_get_attr             ( Rc_string obj, Rc_string name )                                                                       override { if ( scope->want_exec() ) on_get_attr             ( obj, name ); return false; }
    virtual bool enter_test_get_attr_ptr         ( Rc_string obj, Rc_string name )                                                                       override { if ( scope->want_exec() ) on_get_attr_ptr         ( obj, name ); return false; }
    virtual bool enter_test_scope_resolution     ( Rc_string obj, Rc_string name )                                                                       override { if ( scope->want_exec() ) on_scope_resolution     ( obj, name ); return false; }
    virtual bool enter_test_get_attr_calc        ( Rc_string obj, const Vec<Rc_string> &args )                                                           override { if ( scope->want_exec() ) on_get_attr_calc        ( obj, args ); return false; }
    virtual bool enter_test_get_attr_ptr_calc    ( Rc_string obj, const Vec<Rc_string> &args )                                                           override { if ( scope->want_exec() ) on_get_attr_ptr_calc    ( obj, args ); return false; }
    virtual bool enter_test_scope_resolution_calc( Rc_string obj, const Vec<Rc_string> &args )                                                           override { if ( scope->want_exec() ) on_scope_resolution_calc( obj, args ); return false; }
    virtual bool enter_test_block                ( const Vec<Rc_string> &items )                                                                         override { if ( scope->want_exec() ) on_block                ( items ); return false; }
    virtual bool enter_test_apply_op             ( Rc_string f, const Vec<Rc_string> &args )                                                             override { if ( scope->want_exec() ) on_apply_op             ( f, args ); return false; }
    virtual bool enter_test_apply                ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads )    override { if ( scope->want_exec() ) on_apply                ( f, args, names, spreads ); return false; }
    virtual bool enter_test_select               ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads )    override { if ( scope->want_exec() ) on_select               ( f, args, names, spreads ); return false; }
    virtual bool enter_test_chbeba               ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads )    override { if ( scope->want_exec() ) on_chbeba               ( f, args, names, spreads ); return false; }
    virtual bool enter_test_init_of              ( Rc_string name, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) override { if ( scope->want_exec() ) on_init_of              ( name, args, names, spreads ); return false; }
    virtual bool enter_test_assign               ( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags )                      override { if ( scope->want_exec() ) on_assign               ( name, cname, nb_scopes_res, value, flags ); return false; }
    virtual bool enter_test_ss_block             ( const Vec<Rc_string> &items )                                                                         override { if ( scope->want_exec() ) on_ss_block             ( items ); return false; }
    virtual bool enter_test_destruct_assign      ( const Vec<Rc_string> &names, const Vec<Rc_string> &equal, const Vec<Rc_string> &types,
                                                   Rc_string value, PI8 flags )                                                                          override { if ( scope->want_exec() ) on_destruct_assign( names, equal, types, value, flags ); return false; }
    virtual bool enter_test_def                  ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints,
                                                   const Vec<Rc_string> &arg_def_vals,  const Vec<size_t> &arg_spreads,
                                                   const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints,
                                                   const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition,
                                                   Rc_string pertinence,  SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block,
                                                   Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc )                         override { if ( scope->want_exec() ) on_def( name, cname, nb_scopes_rec, arg_names, arg_constraints, arg_def_vals, arg_spreads, with_names, with_constraints, with_def_vals, with_spreads, condition, pertinence, def_pert_num, def_pert_exp, flags, block, self_constraint, return_type, wpc ); return false; }
    virtual bool enter_test_class                ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints,
                                                   const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads,
                                                   const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints,
                                                   const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition,
                                                   Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block,
                                                   const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance )                          override { if ( scope->want_exec() ) on_class      ( name, cname, nb_scopes_rec, arg_names, arg_constraints, arg_def_vals, arg_spreads, with_names, with_constraints, with_def_vals, with_spreads, condition, pertinence, def_pert_num, def_pert_exp, flags, block, inheritance_names, inheritance ); return false; }
    virtual bool enter_test_if                   ( Rc_string cond, const Vec<Rc_string> &ok )                                                            override { if ( scope->want_exec() ) on_if         ( cond, ok ); return false; }
    virtual bool enter_test_if_else              ( Rc_string cond, const Vec<Rc_string> &ok, const Vec<Rc_string> &ko )                                  override { if ( scope->want_exec() ) on_if_else    ( cond, ok, ko ); return false; }
    virtual bool enter_test_while                ( Rc_string cond, const Vec<Rc_string> &ok )                                                            override { if ( scope->want_exec() ) on_while      ( cond, ok ); return false; }
    virtual bool enter_test_return               ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_return     ( value ); return false; }
    virtual bool enter_test_for                  ( Rc_string name, Rc_string container, const Vec<Rc_string> &block )                                    override { if ( scope->want_exec() ) on_for        ( name, container, block ); return false; }
    virtual bool enter_test_typeof               ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_typeof     ( value ); return false; }
    virtual bool enter_test_sizeof               ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_sizeof     ( value ); return false; }
    virtual bool enter_test_aligof               ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_aligof     ( value ); return false; }
    virtual bool enter_test_sizeof_bits          ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_sizeof_bits( value ); return false; }
    virtual bool enter_test_aligof_bits          ( Rc_string value )                                                                                     override { if ( scope->want_exec() ) on_aligof_bits( value ); return false; }
    virtual bool enter_test_lambda               ( const Vec<Rc_string> &names, const Vec<Rc_string> &types, const Vec<Rc_string> &def_vals,
                                                   const Vec<size_t> &spreads, Rc_string body )                                                          override { if ( scope->want_exec() ) on_lambda     ( names, types, def_vals, spreads, body ); return false; }
    virtual bool enter_test_and                  ( Rc_string v0, Rc_string v1 )                                                                          override { if ( scope->want_exec() ) on_and        ( v0, v1 ); return false; }
    virtual bool enter_test_or                   ( Rc_string v0, Rc_string v1 )                                                                          override { if ( scope->want_exec() ) on_or         ( v0, v1 ); return false; }
    virtual bool enter_test_vec                  ( const Vec<Rc_string> &values )                                                                        override { if ( scope->want_exec() ) on_vec        ( values ); return false; }
    virtual bool enter_test_map                  ( const Vec<Rc_string> &names, const Vec<Rc_string> &values )                                           override { if ( scope->want_exec() ) on_map        ( names, values ); return false; }
    virtual bool enter_test_matrix               ( const Vec<Rc_string> &values, const Vec<size_t> &sizes )                                              override { if ( scope->want_exec() ) on_matrix     ( values, sizes ); return false; }
    virtual bool enter_test_raii                 ( const Vec<Rc_string> &code )                                                                           override { if ( scope->want_exec() ) on_raii       ( code ); return false; }
    virtual bool enter_test_info                 ( const Vec<Rc_string> &str, const Vec<Rc_string> &code )                                               override { if ( scope->want_exec() ) on_info       ( str, code ); return false; }
    virtual bool enter_test_assert               ( Rc_string str, Rc_string code )                                                                       override { if ( scope->want_exec() ) on_assert     ( str, code ); return false; }
    virtual bool enter_test_try_catch            ( Rc_string try_body, Rc_string arg_name, Rc_string arg_constraint, Rc_string catch_body, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_splats ) override { if ( scope->want_exec() ) on_try_catch( try_body, arg_name, arg_constraint, catch_body, with_names, with_constraints, with_def_vals, with_splats ); return false; }
    virtual bool enter_test_import               ( Rc_string filename, const Vec<Rc_string> &names, const Vec<Rc_string> &args )                         override { if ( scope->want_exec() ) on_import( filename, names, args ); return false; }
    virtual bool enter_test_enum                 ( Rc_string name, const Vec<Rc_string> &items )                                                         override { if ( scope->want_exec() ) on_enum( name, items ); return false; }

    virtual void on_variable                     ( Rc_string name ) = 0;
    virtual void on_var_assembly                 ( const Vec<Rc_string> &args ) = 0;
    virtual void on_break                        () = 0;
    virtual void on_continue                     () = 0;
    virtual void on_self                         () = 0;
    virtual void on_this                         () = 0;
    virtual void on_pass                         () = 0;
    virtual void on_false                        () = 0;
    virtual void on_true                         () = 0;
    virtual void on_number                       ( Rc_string value ) = 0;
    virtual void on_string                       ( Rc_string value ) = 0;
    virtual void on_get_attr                     ( Rc_string obj, Rc_string name ) = 0;
    virtual void on_get_attr_ptr                 ( Rc_string obj, Rc_string name ) = 0;
    virtual void on_scope_resolution             ( Rc_string obj, Rc_string name ) = 0;
    virtual void on_get_attr_calc                ( Rc_string obj, const Vec<Rc_string> &args ) = 0;
    virtual void on_get_attr_ptr_calc            ( Rc_string obj, const Vec<Rc_string> &args ) = 0;
    virtual void on_scope_resolution_calc        ( Rc_string obj, const Vec<Rc_string> &args ) = 0;
    virtual void on_block                        ( const Vec<Rc_string> &items ) = 0;
    virtual void on_apply_op                     ( Rc_string f, const Vec<Rc_string> &args ) = 0;
    virtual void on_apply                        ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) = 0;
    virtual void on_select                       ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) = 0;
    virtual void on_chbeba                       ( Rc_string f, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) = 0;
    virtual void on_init_of                      ( Rc_string name, const Vec<Rc_string> &args, const Vec<Rc_string> &names, const Vec<size_t> &spreads ) = 0;
    virtual void on_assign                       ( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags ) = 0;
    virtual void on_ss_block                     ( const Vec<Rc_string> &items ) = 0;
    virtual void on_destruct_assign              ( const Vec<Rc_string> &names, const Vec<Rc_string> &equal, const Vec<Rc_string> &types, Rc_string value, PI8 flags ) = 0;
    virtual void on_def                          ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc ) = 0;
    virtual void on_class                        ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance ) = 0;
    virtual void on_if                           ( Rc_string cond, const Vec<Rc_string> &ok ) = 0;
    virtual void on_if_else                      ( Rc_string cond, const Vec<Rc_string> &ok, const Vec<Rc_string> &ko ) = 0;
    virtual void on_while                        ( Rc_string cond, const Vec<Rc_string> &ok ) = 0;
    virtual void on_return                       ( Rc_string value ) = 0;
    virtual void on_for                          ( Rc_string name, Rc_string container, const Vec<Rc_string> &block ) = 0;
    virtual void on_typeof                       ( Rc_string value ) = 0;
    virtual void on_sizeof                       ( Rc_string value ) = 0;
    virtual void on_aligof                       ( Rc_string value ) = 0;
    virtual void on_sizeof_bits                  ( Rc_string value ) = 0;
    virtual void on_aligof_bits                  ( Rc_string value ) = 0;
    virtual void on_lambda                       ( const Vec<Rc_string> &names, const Vec<Rc_string> &types, const Vec<Rc_string> &def_vals, const Vec<size_t> &spreads, Rc_string body ) = 0;
    virtual void on_and                          ( Rc_string v0, Rc_string v1 ) = 0;
    virtual void on_or                           ( Rc_string v0, Rc_string v1 ) = 0;
    virtual void on_vec                          ( const Vec<Rc_string> &values ) = 0;
    virtual void on_map                          ( const Vec<Rc_string> &names, const Vec<Rc_string> &values ) = 0;
    virtual void on_matrix                       ( const Vec<Rc_string> &values, const Vec<size_t> &sizes ) = 0;
    virtual void on_raii                         ( const Vec<Rc_string> &code ) = 0;
    virtual void on_info                         ( const Vec<Rc_string> &str, const Vec<Rc_string> &code ) = 0;
    virtual void on_assert                       ( Rc_string str, Rc_string code ) = 0;
    virtual void on_try_catch                    ( Rc_string try_body, Rc_string arg_name, Rc_string arg_constraint, Rc_string catch_body, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_splats ) = 0;
    virtual void on_import                       ( Rc_string filename, const Vec<Rc_string> &names, const Vec<Rc_string> &args ) = 0;
    virtual void on_enum                         ( Rc_string name, const Vec<Rc_string> &items ) = 0;

    Scope *scope;
};

