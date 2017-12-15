// Generated file
#pragma once

#include <Hpipe/BinStreamWithOffsets.h>
#include <unordered_map>
#include "../Lexem.h"
#include <deque>

#ifndef AST_TOK_NUMS
#define AST_TOK_NUMS
#define AST_TOK_pass_NUM 0
#define AST_TOK_variable_NUM 1
#define AST_TOK_var_assembly_NUM 2
#define AST_TOK_continue_NUM 3
#define AST_TOK_break_NUM 4
#define AST_TOK_false_NUM 5
#define AST_TOK_true_NUM 6
#define AST_TOK_self_NUM 7
#define AST_TOK_this_NUM 8
#define AST_TOK_number_NUM 9
#define AST_TOK_string_NUM 10
#define AST_TOK_get_attr_NUM 11
#define AST_TOK_scope_resolution_NUM 12
#define AST_TOK_get_attr_ptr_NUM 13
#define AST_TOK_get_attr_calc_NUM 14
#define AST_TOK_scope_resolution_calc_NUM 15
#define AST_TOK_get_attr_ptr_calc_NUM 16
#define AST_TOK_ss_block_NUM 17
#define AST_TOK_block_NUM 18
#define AST_TOK_apply_op_NUM 19
#define AST_TOK_apply_NUM 20
#define AST_TOK_select_NUM 21
#define AST_TOK_chbeba_NUM 22
#define AST_TOK_init_of_NUM 23
#define AST_TOK_assign_NUM 24
#define AST_TOK_destruct_assign_NUM 25
#define AST_TOK_def_NUM 26
#define AST_TOK_class_NUM 27
#define AST_TOK_if_else_NUM 28
#define AST_TOK_if_NUM 29
#define AST_TOK_while_NUM 30
#define AST_TOK_return_NUM 31
#define AST_TOK_for_NUM 32
#define AST_TOK_typeof_NUM 33
#define AST_TOK_sizeof_NUM 34
#define AST_TOK_aligof_NUM 35
#define AST_TOK_sizeof_bits_NUM 36
#define AST_TOK_aligof_bits_NUM 37
#define AST_TOK_lambda_NUM 38
#define AST_TOK_and_NUM 39
#define AST_TOK_or_NUM 40
#define AST_TOK_vec_NUM 41
#define AST_TOK_map_NUM 42
#define AST_TOK_matrix_NUM 43
#define AST_TOK_raii_NUM 44
#define AST_TOK_info_NUM 45
#define AST_TOK_assert_NUM 46
#define AST_TOK_try_catch_NUM 47
#define AST_TOK_import_NUM 48
#define AST_TOK_enum_NUM 49
#endif // AST_TOK_NUMS

struct AstWriter {
    struct Apply { Hpipe::CmString attr; Vec<const Lexem *> args; Vec<Hpipe::CmString> arg_names; };
    struct Waiting { PT *first; const Lexem *second; int flags; };
    AstWriter() : bs( &cq ) {}
    void write_pass(  ) {
        bs.write_byte( AST_TOK_pass_NUM );
    }
    void write_variable( const Lexem *lex, const char *name_beg, int name_len, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_variable_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
    }
    void write_var_assembly( const Lexem *lex, const Vec<const Lexem *> &args, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_var_assembly_NUM );
        _write_src( lex );
        _write_VecLex( args, wr_flags );
    }
    void write_continue( const Lexem *lex, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_continue_NUM );
        _write_src( lex );
    }
    void write_break( const Lexem *lex, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_break_NUM );
        _write_src( lex );
    }
    void write_false(  ) {
        bs.write_byte( AST_TOK_false_NUM );
    }
    void write_true(  ) {
        bs.write_byte( AST_TOK_true_NUM );
    }
    void write_self( const Lexem *lex, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_self_NUM );
        _write_src( lex );
    }
    void write_this( const Lexem *lex, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_this_NUM );
        _write_src( lex );
    }
    void write_number( const Lexem *lex, const char *value_beg, int value_len, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_number_NUM );
        _write_src( lex );
        bs.write_String( value_beg, value_len );
    }
    void write_string( const Lexem *lex, const char *value_beg, int value_len, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_string_NUM );
        _write_src( lex );
        bs.write_String( value_beg, value_len );
    }
    void write_get_attr( const Lexem *obj, const char *name_beg, int name_len, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_get_attr_NUM );
        _write_Lex( obj, wr_flags );
        bs.write_String( name_beg, name_len );
    }
    void write_scope_resolution( const Lexem *obj, const char *name_beg, int name_len, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_scope_resolution_NUM );
        _write_Lex( obj, wr_flags );
        bs.write_String( name_beg, name_len );
    }
    void write_get_attr_ptr( const Lexem *obj, const char *name_beg, int name_len, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_get_attr_ptr_NUM );
        _write_Lex( obj, wr_flags );
        bs.write_String( name_beg, name_len );
    }
    void write_get_attr_calc( const Lexem *obj, const Vec<const Lexem *> &args, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_get_attr_calc_NUM );
        _write_Lex( obj, wr_flags );
        _write_VecLex( args, wr_flags );
    }
    void write_scope_resolution_calc( const Lexem *obj, const Vec<const Lexem *> &args, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_scope_resolution_calc_NUM );
        _write_Lex( obj, wr_flags );
        _write_VecLex( args, wr_flags );
    }
    void write_get_attr_ptr_calc( const Lexem *obj, const Vec<const Lexem *> &args, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_get_attr_ptr_calc_NUM );
        _write_Lex( obj, wr_flags );
        _write_VecLex( args, wr_flags );
    }
    void write_ss_block( const Vec<const Lexem *> &items, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_ss_block_NUM );
        _write_VecLex( items, wr_flags );
    }
    void write_block( const Vec<const Lexem *> &items, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_block_NUM );
        _write_VecLex( items, wr_flags );
    }
    void write_apply_op( const Lexem *lex, const char *name_beg, int name_len, const Vec<const Lexem *> &args, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_apply_op_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
        _write_VecLex( args, wr_flags );
    }
    void write_apply( const Lexem *lex, const Lexem *f, const Vec<const Lexem *> &args, const Vec<Hpipe::CmString> &names, const Vec<size_t> &splats, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_apply_NUM );
        _write_src( lex );
        _write_Lex( f, wr_flags );
        _write_VecLex( args, wr_flags );
        _write_VecString( names );
        _write_VecPT( splats );
    }
    void write_select( const Lexem *lex, const Lexem *f, const Vec<const Lexem *> &args, const Vec<Hpipe::CmString> &names, const Vec<size_t> &splats, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_select_NUM );
        _write_src( lex );
        _write_Lex( f, wr_flags );
        _write_VecLex( args, wr_flags );
        _write_VecString( names );
        _write_VecPT( splats );
    }
    void write_chbeba( const Lexem *lex, const Lexem *f, const Vec<const Lexem *> &args, const Vec<Hpipe::CmString> &names, const Vec<size_t> &splats, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_chbeba_NUM );
        _write_src( lex );
        _write_Lex( f, wr_flags );
        _write_VecLex( args, wr_flags );
        _write_VecString( names );
        _write_VecPT( splats );
    }
    void write_init_of( const Lexem *lex, const char *name_beg, int name_len, const Vec<const Lexem *> &args, const Vec<Hpipe::CmString> &names, const Vec<size_t> &splats, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_init_of_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
        _write_VecLex( args, wr_flags );
        _write_VecString( names );
        _write_VecPT( splats );
    }
    void write_assign( const Lexem *lex, const char *name_beg, int name_len, const Lexem *cname, PI8 nb_scope_rec, const Lexem *value, PI8 flags, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_assign_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
        _write_Lex( cname, wr_flags );
        bs.write_byte( nb_scope_rec );
        _write_Lex( value, wr_flags );
        bs.write_byte( flags );
    }
    void write_destruct_assign( const Lexem *lex, const Vec<Hpipe::CmString> &names, const Vec<const Lexem *> &equal, const Vec<const Lexem *> &types, const Lexem *value, PI8 flags, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_destruct_assign_NUM );
        _write_src( lex );
        _write_VecString( names );
        _write_VecLex( equal, wr_flags );
        _write_VecLex( types, wr_flags );
        _write_Lex( value, wr_flags );
        bs.write_byte( flags );
    }
    void write_def( const Lexem *lex, const char *name_beg, int name_len, const Lexem *cname, PI8 nb_scope_rec, const Vec<Hpipe::CmString> &arg_names, const Vec<const Lexem *> &arg_constraints, const Vec<const Lexem *> &arg_def_vals, const Vec<size_t> &arg_splats, const Vec<Hpipe::CmString> &with_names, const Vec<const Lexem *> &with_constraints, const Vec<const Lexem *> &with_def_vals, const Vec<size_t> &with_splats, const Lexem *condition, const Lexem *pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<const Lexem *> &block, const Lexem *self_constraint, const Lexem *return_type, const Vec<const Lexem *> &wpc, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_def_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
        _write_Lex( cname, wr_flags );
        bs.write_byte( nb_scope_rec );
        _write_VecString( arg_names );
        _write_VecLex( arg_constraints, wr_flags );
        _write_VecLex( arg_def_vals, wr_flags );
        _write_VecPT( arg_splats );
        _write_VecString( with_names );
        _write_VecLex( with_constraints, wr_flags );
        _write_VecLex( with_def_vals, wr_flags );
        _write_VecPT( with_splats );
        _write_Lex( condition, wr_flags );
        _write_Lex( pertinence, wr_flags );
        bs.write_signed( def_pert_num );
        bs.write_signed( def_pert_exp );
        bs.write_unsigned( flags );
        _write_VecLex( block, wr_flags );
        _write_Lex( self_constraint, wr_flags );
        _write_Lex( return_type, wr_flags );
        _write_VecLex( wpc, wr_flags );
    }
    void write_class( const Lexem *lex, const char *name_beg, int name_len, const Lexem *cname, PI8 nb_scope_rec, const Vec<Hpipe::CmString> &arg_names, const Vec<const Lexem *> &arg_constraints, const Vec<const Lexem *> &arg_def_vals, const Vec<size_t> &arg_splats, const Vec<Hpipe::CmString> &with_names, const Vec<const Lexem *> &with_constraints, const Vec<const Lexem *> &with_def_vals, const Vec<size_t> &with_splats, const Lexem *condition, const Lexem *pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<const Lexem *> &block, const Vec<Hpipe::CmString> &inheritance_names, const Vec<const Lexem *> &inheritance, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_class_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
        _write_Lex( cname, wr_flags );
        bs.write_byte( nb_scope_rec );
        _write_VecString( arg_names );
        _write_VecLex( arg_constraints, wr_flags );
        _write_VecLex( arg_def_vals, wr_flags );
        _write_VecPT( arg_splats );
        _write_VecString( with_names );
        _write_VecLex( with_constraints, wr_flags );
        _write_VecLex( with_def_vals, wr_flags );
        _write_VecPT( with_splats );
        _write_Lex( condition, wr_flags );
        _write_Lex( pertinence, wr_flags );
        bs.write_signed( def_pert_num );
        bs.write_signed( def_pert_exp );
        bs.write_unsigned( flags );
        _write_VecLex( block, wr_flags );
        _write_VecString( inheritance_names );
        _write_VecLex( inheritance, wr_flags );
    }
    void write_if_else( const Lexem *cond, const Vec<const Lexem *> &ok, const Vec<const Lexem *> &ko, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_if_else_NUM );
        _write_Lex( cond, wr_flags );
        _write_VecLex( ok, wr_flags );
        _write_VecLex( ko, wr_flags );
    }
    void write_if( const Lexem *cond, const Vec<const Lexem *> &ok, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_if_NUM );
        _write_Lex( cond, wr_flags );
        _write_VecLex( ok, wr_flags );
    }
    void write_while( const Lexem *cond, const Vec<const Lexem *> &ok, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_while_NUM );
        _write_Lex( cond, wr_flags );
        _write_VecLex( ok, wr_flags );
    }
    void write_return( const Lexem *lex, const Lexem *value, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_return_NUM );
        _write_src( lex );
        _write_Lex( value, wr_flags );
    }
    void write_for( const Lexem *lex, const char *name_beg, int name_len, const Lexem *container, const Vec<const Lexem *> &block, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_for_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
        _write_Lex( container, wr_flags );
        _write_VecLex( block, wr_flags );
    }
    void write_typeof( const Lexem *lex, const Lexem *value, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_typeof_NUM );
        _write_src( lex );
        _write_Lex( value, wr_flags );
    }
    void write_sizeof( const Lexem *lex, const Lexem *value, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_sizeof_NUM );
        _write_src( lex );
        _write_Lex( value, wr_flags );
    }
    void write_aligof( const Lexem *lex, const Lexem *value, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_aligof_NUM );
        _write_src( lex );
        _write_Lex( value, wr_flags );
    }
    void write_sizeof_bits( const Lexem *lex, const Lexem *value, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_sizeof_bits_NUM );
        _write_src( lex );
        _write_Lex( value, wr_flags );
    }
    void write_aligof_bits( const Lexem *lex, const Lexem *value, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_aligof_bits_NUM );
        _write_src( lex );
        _write_Lex( value, wr_flags );
    }
    void write_lambda( const Lexem *lex, const Vec<Hpipe::CmString> &arg_names, const Vec<const Lexem *> &arg_constraints, const Vec<const Lexem *> &arg_def_vals, const Vec<size_t> &splats, const Lexem *body, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_lambda_NUM );
        _write_src( lex );
        _write_VecString( arg_names );
        _write_VecLex( arg_constraints, wr_flags );
        _write_VecLex( arg_def_vals, wr_flags );
        _write_VecPT( splats );
        _write_Lex( body, wr_flags );
    }
    void write_and( const Lexem *lex, const Lexem *v0, const Lexem *v1, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_and_NUM );
        _write_src( lex );
        _write_Lex( v0, wr_flags );
        _write_Lex( v1, wr_flags );
    }
    void write_or( const Lexem *lex, const Lexem *v0, const Lexem *v1, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_or_NUM );
        _write_src( lex );
        _write_Lex( v0, wr_flags );
        _write_Lex( v1, wr_flags );
    }
    void write_vec( const Vec<const Lexem *> &values, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_vec_NUM );
        _write_VecLex( values, wr_flags );
    }
    void write_map( const Vec<const Lexem *> &keys, const Vec<const Lexem *> &values, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_map_NUM );
        _write_VecLex( keys, wr_flags );
        _write_VecLex( values, wr_flags );
    }
    void write_matrix( const Vec<const Lexem *> &values, const Vec<size_t> &sizes, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_matrix_NUM );
        _write_VecLex( values, wr_flags );
        _write_VecPT( sizes );
    }
    void write_raii( const Lexem *lex, const Vec<const Lexem *> &values, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_raii_NUM );
        _write_src( lex );
        _write_VecLex( values, wr_flags );
    }
    void write_info( const Lexem *lex, const Vec<Hpipe::CmString> &str, const Vec<const Lexem *> &code, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_info_NUM );
        _write_src( lex );
        _write_VecString( str );
        _write_VecLex( code, wr_flags );
    }
    void write_assert( const Lexem *lex, const char *str_beg, int str_len, const Lexem *code, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_assert_NUM );
        _write_src( lex );
        bs.write_String( str_beg, str_len );
        _write_Lex( code, wr_flags );
    }
    void write_try_catch( const Lexem *lex, const Lexem *try_body, const char *arg_name_beg, int arg_name_len, const Lexem *arg_constraint, const Lexem *catch_body, const Vec<Hpipe::CmString> &with_names, const Vec<const Lexem *> &with_constraints, const Vec<const Lexem *> &with_def_vals, const Vec<size_t> &with_splats, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_try_catch_NUM );
        _write_src( lex );
        _write_Lex( try_body, wr_flags );
        bs.write_String( arg_name_beg, arg_name_len );
        _write_Lex( arg_constraint, wr_flags );
        _write_Lex( catch_body, wr_flags );
        _write_VecString( with_names );
        _write_VecLex( with_constraints, wr_flags );
        _write_VecLex( with_def_vals, wr_flags );
        _write_VecPT( with_splats );
    }
    void write_import( const Lexem *lex, const char *filename_beg, int filename_len, const Vec<Hpipe::CmString> &names, const Vec<Hpipe::CmString> &attrs, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_import_NUM );
        _write_src( lex );
        bs.write_String( filename_beg, filename_len );
        _write_VecString( names );
        _write_VecString( attrs );
    }
    void write_enum( const Lexem *lex, const char *name_beg, int name_len, const Vec<Hpipe::CmString> &items, int wr_flags = 0 ) {
        bs.write_byte( AST_TOK_enum_NUM );
        _write_src( lex );
        bs.write_String( name_beg, name_len );
        _write_VecString( items );
    }
    void _write_src( const Lexem *lex ) {
        if ( ! lex || ! lex->src ) {
            bs.write_unsigned( 0 );
            return;
        }
        auto iter = src_map.find( lex->src );
        if ( iter == src_map.end() )
            iter = src_map.insert( iter, std::make_pair( lex->src, src_map.size() + 1 ) );
        bs.write_unsigned( iter->second );
        bs.write_unsigned( lex->off() );
    }
    
    void _write_VecPT( const Vec<size_t> &vl ) {
        bs.write_unsigned( vl.size() );
        for( size_t val : vl )
            bs.write_unsigned( val );
    }
    void _write_VecString( const Vec<Hpipe::CmString> &vl ) {
        bs.write_unsigned( vl.size() );
        for( const Hpipe::CmString &str : vl )
            bs << str;
    }
    void _write_VecVecString( const Vec<Vec<Hpipe::CmString>> &vl ) {
        bs.write_unsigned( vl.size() );
        for( const Vec<Hpipe::CmString> &vs : vl )
            _write_VecString( vs );
    }
    void _write_VecApply( const Vec<Apply> &vl ) {
        bs.write_unsigned( vl.size() );
        for( const Apply &l : vl ) {
            bs << l.attr;
            _write_VecLex( l.args );
            _write_VecString( l.arg_names );
        }
    }
    void _write_VecLex( const Vec<const Lexem *> &vl, int wr_flags = 0 ) {
        bs.write_unsigned( vl.size() );
        for( const Lexem *l : vl )
            _write_Lex( l, wr_flags );
    }
    void _write_Lex( const Lexem *l, int wr_flags = 0 ) {
        if ( l )
            ql.push_back( Waiting{ bs.new_offset(), l, wr_flags } );
        else
            bs.write_unsigned( 0 );
    }
    std::unordered_map<std::string,unsigned>    src_map;
    Hpipe::BinStreamWithOffsets<Hpipe::CbQueue> bs;
    Hpipe::CbQueue                              cq;
    std::deque<Waiting>                         ql;
};
