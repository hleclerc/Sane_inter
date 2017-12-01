#include "ListOfTypesOrSurdefLists.h"
#include "InterpTypeAttribute.h"
#include "System/File_reader.h"
#include "System/Bool_vec.h"
#include "System/BoolRef.h"
#include "Ast_visitor_Vm.h"
#include "AnonymousRoom.h"
#include "System/Memcpy.h"
#include "Ast/Ast_maker.h"
#include "System/rcast.h"
#include "System/Math.h"
#include "Primitives.h"
#include "SurdefList.h"
#include "Type_Union.h"
#include "Varargs.h"
#include "Import.h"
#include "CtInfo.h"
#include "String.h"
#include "Lambda.h"
#include "equal.h"
#include "Class.h"
#include "Scope.h"
#include "Type.h"
#include "Cmp.h"
#include "TCI.h"
#include "Vm.h"
#include "AT.h"

#include "(test_known.ts).h"
#include <experimental/filesystem>
#include <unistd.h>
#include <fcntl.h>

namespace {

Variable make_Variable( Vm *vm, Bool v ) { return { vm, vm->type_Bool, &v }; }
Variable make_Variable( Vm *vm, SI64 v ) { return { vm, vm->type_SI64, &v }; }
Variable make_Variable( Vm *vm, PI64 v ) { return { vm, vm->type_PI64, &v }; }
Variable make_Variable( Vm *vm, SI32 v ) { return { vm, vm->type_SI32, &v }; }
Variable make_Variable( Vm *vm, PI32 v ) { return { vm, vm->type_PI32, &v }; }
//Variable make_Variable( Vm *vm, SI16 v ) { return { vm, vm->type_SI16, &v }; }
//Variable make_Variable( Vm *vm, PI16 v ) { return { vm, vm->type_PI16, &v }; }
//Variable make_Variable( Vm *vm, SI8  v ) { return { vm, vm->type_SI8 , &v }; }
//Variable make_Variable( Vm *vm, PI8  v ) { return { vm, vm->type_PI8 , &v }; }

}

Primitive_decl *last_Primitive_decl = 0;
Primitive_func *last_Primitive_func = 0;

Primitive_decl::Primitive_decl( const char *name, std::function<Type *()> func ) : name( name ), func( func ) {
    prev = last_Primitive_decl;
    last_Primitive_decl = this;
}

Primitive_func::Primitive_func( const char *name, std::function<Variable (Scope *)> func ) : name( name ), func( func ) {
    prev = last_Primitive_func;
    last_Primitive_func = this;
}

#define REG_PRIMITIVE_TYPE( NAME ) \
    class Type_primitive_##NAME : public Type { \
    public: \
        Type_primitive_##NAME() : Type( #NAME ) {} \
        virtual void      write_to_stream( std::ostream &os, const char *data ) const override { os << "__primitive_" #NAME; } \
        virtual void      write_to_stream( std::ostream &os ) const override { os << #NAME; } \
        virtual void      destroy       ( Scope *scope, const Variable &self, bool use_virtual ) override {} \
        virtual Variable  apply          ( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) override; \
    }; \
    Primitive_decl primitive_decl_##NAME( #NAME, []() { return new Type_primitive_##NAME; } ); \
    Variable Type_primitive_##NAME::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor )

#define REG_PRIMITIVE_FUNC( NAME ) \
    Variable primitive_##NAME( Scope *scope ); \
    Primitive_func primitive_func_##NAME( #NAME, primitive_##NAME ); \
    Variable primitive_##NAME( Scope *scope )

REG_PRIMITIVE_TYPE( ct_info ) {
    for( const Variable &arg: args )
        std::cout << *arg.type << "(" << CtInfo( scope, arg ) << ",cpt_use=" << arg.content->cpt_use - 2 << ",ref=" << arg.content.ptr() << ") ";
    std::cout << std::endl;
    return scope->vm->ref_void;
}

#define REG_PRIMITIVE_TYPE_UNA_OP( NAME, OP ) \
    REG_PRIMITIVE_TYPE( NAME ) { \
        if ( args.size() != 1 ) \
            return scope->add_error( "__primitive_" #NAME " expects exactly 1 argument" ), scope->vm->ref_error; \
        Variable a = args[ 0 ].ugs( scope ); \
        if ( Variable res = test_known( scope->vm, a, [scope]( auto a ) { return make_Variable( scope->vm, OP ); }, Variable{} ) ) \
            return res; \
        return scope->add_error( "type '{}' not managed by __primitive_" #NAME, *args[ 0 ].type ), scope->vm->ref_error; \
    }
REG_PRIMITIVE_TYPE_UNA_OP( neg, - a )
REG_PRIMITIVE_TYPE_UNA_OP( not_boolean, ! a )
REG_PRIMITIVE_TYPE_UNA_OP( not_bitwise, ! a )

#define REG_PRIMITIVE_TYPE_BIN_OP( NAME, OP ) \
    REG_PRIMITIVE_TYPE( NAME ) { \
        if ( args.size() != 2 ) \
            return scope->add_error( "__primitive_" #NAME " expects exactly 2 argument" ), scope->vm->ref_error; \
        Variable a = args[ 0 ].ugs( scope ), b = args[ 1 ].ugs( scope ); \
        if ( Variable res = test_known( scope->vm, a, b, [scope]( auto a, auto b ) { return make_Variable( scope->vm, OP ); }, Variable{} ) ) \
            return res; \
        return scope->add_error( "arg types '{}' and '{}' not managed by __primitive_" #NAME, *a.type, *b.type ), scope->vm->ref_error; \
    }
REG_PRIMITIVE_TYPE_BIN_OP( add        , a + b )
REG_PRIMITIVE_TYPE_BIN_OP( sub        , a - b )
REG_PRIMITIVE_TYPE_BIN_OP( mul        , a * b )
REG_PRIMITIVE_TYPE_BIN_OP( mod        , a % b )
REG_PRIMITIVE_TYPE_BIN_OP( div        , a / b )
REG_PRIMITIVE_TYPE_BIN_OP( div_int    , a / b )
REG_PRIMITIVE_TYPE_BIN_OP( or_bitwise , a | b )
REG_PRIMITIVE_TYPE_BIN_OP( xor_bitwise, a ^ b )
REG_PRIMITIVE_TYPE_BIN_OP( and_bitwise, a & b )
REG_PRIMITIVE_TYPE_BIN_OP( shift_right, a >> b )
REG_PRIMITIVE_TYPE_BIN_OP( shift_left , a << b )
REG_PRIMITIVE_TYPE_BIN_OP( inf        , (Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_inf    ( a, b )) )
REG_PRIMITIVE_TYPE_BIN_OP( sup        , (Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_sup    ( a, b )) )
REG_PRIMITIVE_TYPE_BIN_OP( inf_equ    , (Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_inf_equ( a, b )) )
REG_PRIMITIVE_TYPE_BIN_OP( sup_equ    , (Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_sup_equ( a, b )) )
REG_PRIMITIVE_TYPE_BIN_OP( not_equ    , (Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_not_equ( a, b )) )
REG_PRIMITIVE_TYPE_BIN_OP( equ        , (Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_equ    ( a, b )) )


REG_PRIMITIVE_TYPE( add_AT ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_add_AT expects exactly 2 argument" ), scope->vm->ref_error;
    Variable va = args[ 0 ].ugs( scope );
    Variable vb = args[ 1 ].ugs( scope );
    if ( va.type != scope->vm->type_AT )
        return scope->add_error( "__primitive_add_AT expects an AT as first argument" ), scope->vm->ref_error;
    AT *a = rcast( va.ptr() );
    if ( Variable res = test_known( scope->vm, vb, [a,scope]( auto b ) {
                                    Variable res( scope->vm, scope->vm->type_AT );
                                    AT *r = rcast( res.ptr() );
                                    r->ptr->content = a->ptr->content;
                                    r->ptr->offset_in_bytes = a->ptr->offset_in_bytes + b;
                                    return res; }, Variable{} ) )
        return res;
    return scope->add_error( "arg type '{}' not managed for second arg of __primitive_add_AT", *vb.type ), scope->vm->ref_error;
}

REG_PRIMITIVE_TYPE( sub_AT ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_add_AT expects exactly 2 argument" ), scope->vm->ref_error;
    Variable va = args[ 0 ].ugs( scope );
    Variable vb = args[ 1 ].ugs( scope );
    if ( va.type != scope->vm->type_AT )
        return scope->add_error( "__primitive_add_AT expects an AT as first argument" ), scope->vm->ref_error;
    if ( vb.type != scope->vm->type_AT )
        return scope->add_error( "__primitive_add_AT expects an AT as first argument" ), scope->vm->ref_error;
    AT *a = rcast( va.ptr() );
    AT *b = rcast( vb.ptr() );
    return scope->vm->new_PT( a->ptr->content->data + a->ptr->offset_in_bytes - ( b->ptr->content->data + b->ptr->offset_in_bytes ) );
}

REG_PRIMITIVE_TYPE( reassign ) {
    Variable va = args[ 0 ].ugs( scope );
    Variable vb = args[ 1 ].ugs( scope );

    // for valgrind
    //if ( va.type == scope->vm->type_Bool )
    //    *reinterpret_cast<Bool *>( va.content->data + va.off / 8 ) = 0;

    // primitive numbers
    if ( test_known( scope->vm, va, vb, [&]( auto &a, auto b ) { a = b; return 1; }, 0 ) )
        return args[ 0 ];

    // AT
    if ( va.type == scope->vm->type_AT ) {
        AT *ap = rcast( va.ptr() );
        if ( vb.type == scope->vm->type_AT ) {
            AT *bp = rcast( vb.ptr() );
            ap->ptr->content         = bp->ptr->content;
            ap->ptr->offset_in_bytes = bp->ptr->offset_in_bytes;
        } else if ( vb.type == scope->vm->type_NullPtr ) {
            ap->ptr->content         = 0;
            ap->ptr->offset_in_bytes = 0;
        } else
            TODO;
        return args[ 0 ];
    }

    // char = ...
    if ( va.type == scope->vm->type_Char ) {
        if ( test_known( scope->vm, vb, [&va]( auto b ) { *reinterpret_cast<char *>( va.ptr() ) = b; return 1; }, 0 ) )
            return args[ 0 ];
        if ( vb.type == scope->vm->type_Char ) {
            *reinterpret_cast<char *>( va.ptr() ) = *reinterpret_cast<char *>( vb.ptr() );
            return args[ 0 ];
        }

    }

    // ... = char
    if ( vb.type == scope->vm->type_Char ) {
        if ( test_known( scope->vm, va, [&vb]( auto &a ) { a = *reinterpret_cast<const char *>( vb.ptr() ); return 1; }, 0 ) )
            return args[ 0 ];
    }

    // type
    if ( va.type == scope->vm->type_Type ) {
        if ( vb.type == scope->vm->type_Type ) {
            *reinterpret_cast<Type **>( va.ptr() ) = *reinterpret_cast<Type **>( vb.ptr() );
            return args[ 0 ];
        }
        if ( vb.type == scope->vm->type_SurdefList ) {
            Type *type = vb.apply( scope, true, {}, {}, false ).type;
            *reinterpret_cast<Type **>( va.ptr() ) = type;
            return args[ 0 ];
        }
    }

    if ( va.error() == false && vb.error() == false )
        scope->add_error( "don't known how to reassign {} and {} types", *va.type, *vb.type );
    return args[ 0 ];
}

REG_PRIMITIVE_TYPE( destroy ) {
    Variable va = args[ 0 ].ugs( scope );
    if ( va.type == scope->vm->type_AT ) {
        AT *at = rcast( va.ptr() );
        at->~AT();
        return scope->vm->ref_void;
    }
    TODO;
    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( construct ) {
    Variable va = args[ 0 ].ugs( scope );

    if ( args.size() == 1 ) {
        if ( va.type == scope->vm->type_AT ) {
            AT *ap = new ( va.ptr() ) AT;
            ap->ptr->content         = 0;
            ap->ptr->offset_in_bytes = 0;
            return args[ 0 ];
        }
        TODO;
    }

    Variable vb = args[ 1 ].ugs( scope );
    if ( vb.error() )
        return args[ 0 ];

    // primitive numbers
    if ( test_known( scope->vm, va, vb, [&]( auto &a, auto b ) { a = b; return 1; }, 0 ) )
        return args[ 0 ];

    // AT
    if ( va.type == scope->vm->type_AT ) {
        if ( vb.type == scope->vm->type_NullPtr ) {
            AT *ap = rcast( va.ptr() );
            ap->ptr = new AT::V;
            ap->ptr->content         = 0;
            ap->ptr->offset_in_bytes = 0;
            return args[ 0 ];
        }
        if ( vb.type == scope->vm->type_AT ) {
            AT *ap = rcast( va.ptr() );
            AT *bp = rcast( vb.ptr() );
            ap->ptr = new AT::V( *bp->ptr );
            return args[ 0 ];
        }
    }

    // Bool and ...
    if ( va.type == scope->vm->type_Bool ) {
        Bool *ap = rcast( va.ptr() ); // TODO: handle bools with offsets
        if ( vb.type == scope->vm->type_AT ) {
            AT *bp = rcast( vb.ptr() );
            *ap = bp->ptr->content;
            return args[ 0 ];
        }
    }

    //
    if ( va.type == scope->vm->type_UntypedRef ) {
        if ( vb.type == scope->vm->type_UntypedRef ) {
            Variable *b = rcast( vb.ptr() );
            new ( va.ptr() ) Variable( *b );
            return args[ 0 ];
        } else
            TODO;
    }

    // type
    if ( va.type == scope->vm->type_Type ) {
        if ( vb.type == scope->vm->type_Type ) {
            *reinterpret_cast<Type **>( va.ptr() ) = *reinterpret_cast<Type **>( vb.ptr() );
            return args[ 0 ];
        }
        if ( vb.type == scope->vm->type_SurdefList ) {
            Type *type = vb.apply( scope, true, {}, {}, false ).type;
            *reinterpret_cast<Type **>( va.ptr() ) = type;
            return args[ 0 ];
        }
    }

    scope->add_error( "don't known how to init a {} with a {}", *va.type, *vb.type );
    return args[ 0 ];
}

REG_PRIMITIVE_TYPE( sizeof_ptr_in_bits ) {
    return scope->vm->new_PT( 8 * sizeof( void * ) );
}

REG_PRIMITIVE_TYPE( aligof_ptr_in_bits ) {
    return scope->vm->new_PT( 8 * sizeof( void * ) );
}

REG_PRIMITIVE_TYPE( is_little_endian ) {
    return scope->vm->new_Bool( __BYTE_ORDER ==__LITTLE_ENDIAN );
}

REG_PRIMITIVE_TYPE( has_fixed_size ) {
    return scope->vm->new_Bool( true );
}

REG_PRIMITIVE_TYPE( read_sync ) {
    Variable a = args[ 0 ].ugs( scope );
    Rc_string r = *reinterpret_cast<String *>( a.ptr() );
    File_reader fr( r );
    if ( fr )
        return scope->vm->new_String( fr.data );
    return args[ 1 ].ugs( scope ).apply( scope, want_ret );
}

REG_PRIMITIVE_TYPE( canonical ) {
    Variable vf = args[ 0 ].ugs( scope );
    Variable vc = args[ 1 ].ugs( scope );
    Variable bc = args[ 2 ].ugs( scope );

    Rc_string f = *reinterpret_cast<String *>( vf.ptr() );
    Rc_string c = *reinterpret_cast<String *>( vc.ptr() );
    try {
        return scope->vm->new_String( std::experimental::filesystem::canonical( std::string( f ), std::string( c ) ).string() );
    } catch ( std::experimental::filesystem::filesystem_error ) {
        return scope->vm->new_String( {} );
    }
}

REG_PRIMITIVE_TYPE( get_cwd ) {
    Variable res( scope->vm, scope->vm->type_String );
    reinterpret_cast<String *>( res.ptr() )->init( std::experimental::filesystem::current_path().string() );
    return res;
}

REG_PRIMITIVE_TYPE( get_argv ) {
    Variable res = scope->find_variable( "Vec" ).select( scope, true, scope->vm->new_Type( scope->vm->type_String ) ).apply( scope, true );
    for( int i = 0; i < scope->vm->argc; ++i )
        res.find_attribute( scope, "push_back" ).apply( scope, false, scope->vm->new_String( scope->vm->argv[ i ] ) );
    return res;
}

REG_PRIMITIVE_TYPE( get_size ) {
    if ( args.size() == 1 ) {
        Variable a = args[ 0 ].ugs( scope );
        if ( a.type == scope->vm->type_Varargs )
            return scope->vm->new_PT( reinterpret_cast<Varargs *>( a.ptr() )->values.size() - reinterpret_cast<Varargs *>( a.ptr() )->names.size() );
    }
    scope->add_error( "unknown case for __primitive_get_size" );
    return scope->vm->ref_error;
}

REG_PRIMITIVE_TYPE( get_attr ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_get_attr expects exactly 2 arguments" ), scope->vm->ref_error;

    // get name
    Variable name_var = args[ 1 ].ugs( scope );
    if ( name_var.type != scope->vm->type_String )
        return scope->add_error( "__primitive_get_attr expects a String as 2nd arguments" ), scope->vm->ref_error;
    Rc_string name = *reinterpret_cast<String *>( name_var.ptr() );

    // generic case
    if ( Variable res = args[ 0 ].find_attribute( scope, name, false ) )
        return res;
    return scope->add_error( "there's no attribute {} in objects of type {}", *args[ 0 ].ugs_type( scope ) ), scope->vm->ref_error;
}

REG_PRIMITIVE_TYPE( get_ref ) {
    if ( args.size() == 1 ) {
        Variable var = args[ 0 ].ugs( scope );
        if ( var.type == scope->vm->type_UntypedRef )
            return *reinterpret_cast<Variable *>( var.ptr() );
        else
            TODO;
    }

    if ( args.size() != 2 )
        return scope->add_error( "__primitive_get_ref expects exactly 2 arguments" ), scope->vm->ref_error;
    Type *type = args[ 0 ].ugs( scope ).apply( scope, true, {}, {}, false ).type;

    Variable atv = args[ 1 ].ugs( scope );
    if ( atv.type != scope->vm->type_AT )
        return scope->add_error( "__primitive_get_ref expects an AT as second argument" ), scope->vm->ref_error;
    AT *at = rcast( atv.ptr() );
    return { at->ptr->content, Variable::Flags::NONE, type, (SI32)at->ptr->offset_in_bytes };
}

REG_PRIMITIVE_TYPE( pointer_on ) {
    if ( args.size() != 1 )
        return scope->add_error( "__primitive_pointer_on expects exactly 1 argument" ), scope->vm->ref_error;
    Variable obj = args[ 0 ].ugs( scope );
    Variable vty( scope->vm, scope->vm->type_Type, &obj.type );
    Variable vat( scope->vm, scope->vm->type_AT );
    AT *at = rcast( vat.ptr() );
    at->ptr->content = obj.content.ptr();
    at->ptr->offset_in_bytes = obj.offB;
    return scope->find_variable( "Ptr" ).select( scope, true, vty ).apply( scope, true, vat );
}

REG_PRIMITIVE_TYPE( constified ) {
    if ( args.size() != 1 )
        return scope->add_error( "__primitive_pointer_on expects exactly 1 argument" ), scope->vm->ref_error;
    Variable a = args[ 0 ];
    a.flags |= Variable::Flags::CONST;
    return a;
}

static void _write( int fd, const char *data, size_t size ) {
    if ( fd == 0 )
        std::cout.write( data, size );
    else if ( fd == 1 )
        std::cerr.write( data, size );
    else {
        // TODO: use errno and so on
        // while ( size ) {
        size_t sent = write( fd, data, size );
        if ( sent == size_t( -1 ) )
            return;
        size -= sent;
        data += sent;
        // }
    }
}

REG_PRIMITIVE_TYPE( write_fd ) {
    if ( args.size() < 2 )
        return scope->add_error( "__primitive_write_fd expects at least 2 arguments" ), scope->vm->ref_error;
    Variable fd = args[ 0 ].ugs( scope );
    if ( fd.type != scope->vm->type_SI32 )
        return scope->add_error( "__primitive_write_fd expects a SI32 as first argument" ), scope->vm->ref_error;
    Variable val = args[ 1 ].ugs( scope );

    if ( args.size() == 3 ) {
        Variable size_var = args[ 2 ].ugs( scope );
        AT *at = rcast( val.ptr() );
        size_t size = 0;
        if ( test_known( scope->vm, size_var, [&size]( auto s ) { size = s; return 0; }, 1 ) )
            scope->add_error( "type {} no handled for second argument of write_fd", *size_var.type );
        _write( *reinterpret_cast<SI32 *>( args[ 0 ].ptr() ), at->ptr->content->data + at->ptr->offset_in_bytes, size );
    } else {
        // TODO: something faster
        std::ostringstream os;
        val.type->write_to_stream( os, val.content->data + val.offB );
        std::string str = os.str();
        _write( *reinterpret_cast<SI32 *>( fd.ptr() ), str.data(), str.size() );
    }

    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( convert_String ) {
    Variable res( scope->vm, scope->vm->type_String );
    Variable var = args[ 0 ].ugs( scope );
    String *s = rcast( res.ptr() );

    std::ostringstream os;
    os << var;
    // if ( test_known( scope->vm, var, [&os]( auto s ) { os << s; return 0; }, 1 ) )
    //     scope->add_error( "type {} no handled for convert_String", *var.type );
    s->init( os.str() );

    return res;
}

REG_PRIMITIVE_TYPE( add_room_in_type ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_add_room_in_type expects exactly 2 arguments" ), scope->vm->ref_error;
    PT size, alig;
    if ( ! args[ 0 ].get_PT_value( scope, size ) || ! args[ 1 ].get_PT_value( scope, alig ) )
        return scope->add_error( "__primitive_add_room_in_type expexts arguments convertible to PT" ), scope->vm->ref_error;

    // add an AnonymousRoom variable in the scope
    Variable ar_var( scope->vm, scope->vm->type_AnonymousRoom );
    new ( ar_var.ptr() ) AnonymousRoom{ size, alig };
    scope->reg_var( to_string( scope->variables.size() ), ar_var );
    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( _union ) {
    if ( args.size() != names.size() )
        return scope->add_error( "union expects only named arguments" ), scope->vm->ref_error;
    Vec<Type *> types;
    SI32 max_size = 0, max_alig = 1;
    for( size_t i = 0; i < args.size(); ++i ) {
        types << const_cast<Variable &>( args[ i ] ).apply( scope, true, {}, {}, false ).type;
        max_size = std::max( max_size, types.back()->size );
        max_alig = lcm( max_alig, types.back()->alig );
    }
    Type *res = scope->vm->types.push_back_val( new Type_Union( max_size, max_alig ) );
    for( size_t i = 0; i < args.size(); ++i )
        res->add_attribute( names[ i ], 0, types[ i ] );
    return { scope->vm, scope->vm->type_Type, &res };
}

REG_PRIMITIVE_TYPE( load ) {
    if ( args.size() != 1 )
        return scope->add_error( "__primitive_load expects exactly 1 argument" ), scope->vm->ref_error;
    Variable filename = args[ 0 ].ugs( scope );
    if ( filename.type != scope->vm->type_String )
        return scope->add_error( "__primitive_load expects a String as first argument (not a {})", *filename.type ), scope->vm->ref_error;
    String *str = rcast( filename.ptr() );
    std::experimental::filesystem::path orig( ( scope->parent ? scope->parent : scope )->src_name( scope->pos.cur_src ) );
    return scope->vm->import( *str, orig.parent_path().string() );
}

REG_PRIMITIVE_TYPE( memcpy ) {
    if ( args.size() != 3 )
        return scope->add_error( "__primitive_memcpy expects exactly 3 arguments" ), scope->vm->ref_error;
    Variable dst = args[ 0 ].ugs( scope );
    Variable src = args[ 1 ].ugs( scope );
    if ( dst.type != scope->vm->type_AT || src.type != scope->vm->type_AT )
        return scope->add_error( "__primitive_memcpy expects ATs as first and second arguments" ), scope->vm->ref_error;
    PT len;
    if ( ! args[ 2 ].ugs( scope ).get_PT_value( scope, len ) )
        return scope->add_error( "__primitive_memcpy expects a third argument convertible to PT" ), scope->vm->ref_error;
    AT *a = rcast( dst.ptr() );
    AT *b = rcast( src.ptr() );

    // scope->disp_pos( "..." );

    if ( len ) {
        if ( a->ptr->offset_in_bytes + len > a->ptr->content->rese )
            scope->add_error( "wrong write" );
        if ( b->ptr->offset_in_bytes + len > b->ptr->content->rese )
            scope->add_error( "wrong read" );
    }
    memcpy( a->ptr->content->data + a->ptr->offset_in_bytes, b->ptr->content->data + b->ptr->offset_in_bytes, len );
    return args[ 0 ];
}

REG_PRIMITIVE_TYPE( memequ ) {
    if ( args.size() != 3 )
        return scope->add_error( "__primitive_memequ expects exactly 3 arguments" ), scope->vm->ref_error;
    Variable dst = args[ 0 ].ugs( scope );
    Variable src = args[ 1 ].ugs( scope );
    if ( dst.type != scope->vm->type_AT || src.type != scope->vm->type_AT )
        return scope->add_error( "__primitive_memequ expects ATs as first and second arguments" ), scope->vm->ref_error;
    PT len;
    if ( ! args[ 2 ].ugs( scope ).get_PT_value( scope, len ) )
        return scope->add_error( "__primitive_memequ expects a third argument convertible to PT" ), scope->vm->ref_error;
    AT *a = rcast( dst.ptr() );
    AT *b = rcast( src.ptr() );
    return scope->vm->new_Bool( memcmp_bit( a->ptr->content->data, 8 * a->ptr->offset_in_bytes, b->ptr->content->data, 8 * b->ptr->offset_in_bytes, 8 * len ) == 0 );
}

REG_PRIMITIVE_TYPE( inherits ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_inherits expects exactly 2 arguments" ), scope->vm->ref_error;
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );
    Type *ta = a.apply( scope, true, {}, {}, false ).type;
    Type *tb = b.apply( scope, true, {}, {}, false ).type;
    return scope->vm->new_Bool( ta->inherits( scope, tb ) );
}

REG_PRIMITIVE_TYPE( alloc ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_alloc expects exactly 2 arguments" ), scope->vm->ref_error;
    PT len, ali;
    if ( ! args[ 0 ].ugs( scope ).get_PT_value( scope, len ) || ! args[ 1 ].ugs( scope ).get_PT_value( scope, ali ) )
        return scope->add_error( "__primitive_alloc expects PT arguments" ), scope->vm->ref_error;
    Variable res( scope->vm, scope->vm->type_AT );
    AT *at = rcast( res.ptr() );
    at->ptr->content = Variable::Content::alloc( len ); // no type for this variable
    at->ptr->content->cpt_use = 100; //
    at->ptr->offset_in_bytes = 0;
    return res;
}

REG_PRIMITIVE_TYPE( free ) {
    if ( args.size() != 3 )
        return scope->add_error( "__primitive_free expects exactly 3 arguments" ), scope->vm->ref_error;
    PT len, ali;
    if ( ! args[ 1 ].ugs( scope ).get_PT_value( scope, len ) || ! args[ 2 ].ugs( scope ).get_PT_value( scope, ali ) )
        return scope->add_error( "__primitive_free expects PT likes as 2nd and 3rd arguments" ), scope->vm->ref_error;
    Variable ptr = args[ 0 ].ugs( scope );
    if ( ptr.type != scope->vm->type_AT )
        return scope->add_error( "__primitive_free expects an AT likes as 1st arguments" ), scope->vm->ref_error;
    AT *at = rcast( ptr.ptr() );
    free( at->ptr->content );
    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( gen_equ ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_gen_equ expects exactly 2 argument" ), scope->vm->ref_error;
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );
    return scope->vm->new_Bool( equal( scope, a, b ) );
}

REG_PRIMITIVE_TYPE( gen_inf_equ ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_inf_equ expects exactly 2 argument" ), scope->vm->ref_error;
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );
    if ( a.type == scope->vm->type_AT ) {
        AT *av = rcast( a.ptr() );
        if ( b.type == scope->vm->type_NullPtr )
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes <= 0 );
        if ( b.type == scope->vm->type_AT ) {
            AT *bv = rcast( b.ptr() );
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes <= bv->ptr->content + bv->ptr->offset_in_bytes );
        }
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    } else {
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    }
    return args[ 0 ];
}

REG_PRIMITIVE_TYPE( gen_sup_equ ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_sup_equ expects exactly 2 argument" ), scope->vm->ref_error;
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );
    if ( a.type == scope->vm->type_AT ) {
        AT *av = rcast( a.ptr() );
        if ( b.type == scope->vm->type_NullPtr )
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes >= 0 );
        if ( b.type == scope->vm->type_AT ) {
            AT *bv = rcast( b.ptr() );
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes >= bv->ptr->content + bv->ptr->offset_in_bytes );
        }
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    } else {
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    }
    return args[ 0 ];
}

REG_PRIMITIVE_TYPE( gen_inf ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_inf expects exactly 2 argument" ), scope->vm->ref_error;
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );
    if ( a.type == scope->vm->type_AT ) {
        AT *av = rcast( a.ptr() );
        if ( b.type == scope->vm->type_NullPtr )
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes < 0 );
        if ( b.type == scope->vm->type_AT ) {
            AT *bv = rcast( b.ptr() );
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes < bv->ptr->content + bv->ptr->offset_in_bytes );
        }
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    } else {
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    }
    return args[ 0 ];
}

REG_PRIMITIVE_TYPE( gen_sup ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_sup expects exactly 2 argument" ), scope->vm->ref_error;
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );
    if ( a.type == scope->vm->type_AT ) {
        AT *av = rcast( a.ptr() );
        if ( b.type == scope->vm->type_NullPtr )
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes > 0 );
        if ( b.type == scope->vm->type_AT ) {
            AT *bv = rcast( b.ptr() );
            return scope->vm->new_Bool( av->ptr->content + av->ptr->offset_in_bytes > bv->ptr->content + bv->ptr->offset_in_bytes );
        }
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    } else {
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    }
    return args[ 0 ];
}

REG_PRIMITIVE_TYPE( make_lotosl ) {
    if ( args.size() != 2 )
        return scope->add_error( "__primitive_make_lotosl expects exactly 2 arguments" ), scope->vm->ref_error;
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );
    if ( a.type == scope->vm->type_SurdefList ) {
        if ( b.type == scope->vm->type_SurdefList ) {
            Variable res( scope->vm, scope->vm->type_ListOfTypesOrSurdefLists );
            ListOfTypesOrSurdefLists *l = rcast( res.ptr() );
            l->lst << a;
            l->lst << b;
            return res;
        } else {
            TODO;
        }
    } else {
        TODO;
    }

    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( inheritance_rec ) {
    // get type children type
    Variable slf = args[ 0 ].ugs( scope );
    Type *type = 0;

    if ( slf.type == scope->vm->type_Type )
        type = *reinterpret_cast<Type **>( slf.ptr() );
    else
        type = slf.apply( scope, true, {}, {}, false ).type;

    // get inheritance list
    std::set<Type *> set;
    type->inheritance_rec( set );

    // store it in a CtVec
    Variable res( scope->vm, scope->vm->type_CtVec );
    LVec<Variable> *vec = rcast( res.ptr() );
    vec->reserve( set.size() );
    for( Type *t : set ) {
        Variable v( scope->vm, scope->vm->type_Type );
        *reinterpret_cast<Type **>( v.ptr() ) = t;
        *vec << v;
    }
    return res;
}

REG_PRIMITIVE_TYPE( scope_add ) {
    Variable sv = args[ 0 ].ugs( scope );
    Variable nv = args[ 1 ].ugs( scope );
    Variable vv = args[ 2 ].ugs( scope );
    Scope **s = rcast( sv.ptr() );
    String *n = rcast( nv.ptr() );
    return Ast_visitor_Vm::assign( *s, *n, [&]() { return vv.apply( scope, true ); }, args[ 3 ].ugs( scope ).is_true( scope ) ? ASSIGN_FLAG_static : 0 );
}

REG_PRIMITIVE_TYPE( scope_declare ) {
    Variable sv = args[ 0 ].ugs( scope );
    Variable nv = args[ 1 ].ugs( scope );
    Variable vv = args[ 2 ].ugs( scope );
    Scope **s = rcast( sv.ptr() );
    String *n = rcast( nv.ptr() );
    return Ast_visitor_Vm::assign( *s, *n, [&]() { return vv.apply( scope, true ); }, ASSIGN_FLAG_type );
}

REG_PRIMITIVE_TYPE( scope_disp_info ) {
    Variable sv = args[ 0 ].ugs( scope );
    Scope **sp = rcast( sv.ptr() );
    for( Scope *s = *sp; s; s = s->parent_for_vars() ) {
        std::cerr << "Scope:";
        for( Scope::NV *nv = s->last_var; nv; nv = nv->prev )
            if ( nv->name )
                std::cerr << " " << nv->name;
        std::cerr << "\n";

        break;

        if ( s->self ) {
            Type *type = s->self.ugs_type( s );
            std::cerr << "Self (type=" << *type << "):";
            type->disp_attributes( std::cerr );
            std::cerr << "\n";
        }
    }
    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( scope_disp_pos ) {
    Variable sv = args[ 0 ].ugs( scope );
    Scope **sp = rcast( sv.ptr() );
    (*sp)->disp_pos( "" );
    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( scope_get_parent ) {
    Variable sv = args[ 0 ].ugs( scope );
    Scope **s = rcast( sv.ptr() );

    Variable res( scope->vm, scope->vm->type_InterpScope );
    Scope **r = rcast( res.ptr() );
    *r = (*s)->parent_interp( true );

    return res;
}

REG_PRIMITIVE_TYPE( make_UntypedRef ) {
    Variable res( scope->vm, scope->vm->type_UntypedRef );
    Variable *v = rcast( res.ptr() );
    *v = args[ 0 ];
    return res;
}

REG_PRIMITIVE_TYPE( rcast ) {
    Type *type = args[ 0 ].ugs( scope ).apply( scope, true, {}, {}, false ).type;
    Variable var = args[ 1 ].ugs( scope );
    return { var.content, var.flags, type, var.offB };
}

REG_PRIMITIVE_TYPE( is_a ) {
    TCI tci;
    Rc_string res = args[ 1 ].ugs( scope ).valid_constraint_for( scope, args[ 0 ].ugs( scope ), tci );
    return scope->vm->new_Bool( res.empty() );
}

REG_PRIMITIVE_TYPE( system ) {
    Variable a = args[ 0 ].ugs( scope );
    String *s = rcast( a.ptr() );
    Rc_string ss = *s;
    std::string sss = ss;
    return scope->vm->new_SI32( system( sss.c_str() ) );
}

REG_PRIMITIVE_TYPE( open ) {
    Variable a = args[ 0 ].ugs( scope );
    String *s = rcast( a.ptr() );
    Rc_string ss = *s;
    std::string sss = ss;
    return scope->vm->new_SI32( open( sss.c_str(), O_RDWR | O_CREAT, S_IRWXU ) );
}

REG_PRIMITIVE_TYPE( throw ) {
    throw scope->exception( args[ 0 ] );
}

REG_PRIMITIVE_TYPE( copy ) {
    Variable a = args[ 0 ].ugs( scope );
    Variable res( scope->vm, a.type );

    if ( a.type == scope->vm->type_Type )
        *reinterpret_cast<Type **>( res.ptr() ) = *reinterpret_cast<Type **>( a.ptr() );
    else if ( a.type == scope->vm->type_Lambda )
        *reinterpret_cast<Lambda *>( res.ptr() ) = *reinterpret_cast<Lambda *>( a.ptr() );
    else if ( a.type == scope->vm->type_SurdefList )
        *reinterpret_cast<SurdefList *>( res.ptr() ) = *reinterpret_cast<SurdefList *>( a.ptr() );
    else
        TODO;

    return res;
}

REG_PRIMITIVE_TYPE( base_class_name ) {
    Variable a = args[ 0 ].ugs( scope );

    // get type of left hand side
    Type *t = 0;
    if ( a.type == scope->vm->type_Type )
        t = *reinterpret_cast<Type **>( a.ptr() );
    else // if ( a.type == scope->vm->type_SurdefList )
        t = a.apply( scope, true, {}, {}, false ).type;

    // return class
    Class *c = t->orig_class();
    return scope->vm->new_String( c ? c->name : "no orig class" );
}

REG_PRIMITIVE_TYPE( has_base_class ) {
    Variable a = args[ 0 ].ugs( scope );
    Variable b = args[ 1 ].ugs( scope );

    // get type of left hand side
    Type *t = 0;
    if ( a.type == scope->vm->type_Type )
        t = *reinterpret_cast<Type **>( a.ptr() );
    else // if ( a.type == scope->vm->type_SurdefList )
        t = a.apply( scope, true, {}, {}, false ).type;

    // inh types
    std::set<Type *> inh_types;
    t->inheritance_rec( inh_types, false );

    // inh classes
    std::set<Class *> inh_classes;
    for( Type *type : inh_types )
        if ( Class *cl = type->orig_class() )
            inh_classes.insert( cl );

    auto test_sl = [&]( SurdefList *sl ) {
        for( const Variable &var : sl->lst ) {
            if ( var.type == scope->vm->type_Class ) {
                Class *bcl = rcast( var.ptr() );
                if ( inh_classes.count( bcl ) )
                    return true;
            }
        }
        return false;
    };

    // type cases for b
    if ( b.type == scope->vm->type_SurdefList )
        return scope->vm->new_Bool( test_sl( rcast( b.ptr() ) ) );

    if ( b.type == scope->vm->type_ListOfTypesOrSurdefLists ) {
        ListOfTypesOrSurdefLists *l = rcast( b.ptr() );
        for( const Variable &v : l->lst )
            if ( test_sl( rcast( v.ptr() ) ) )
                return scope->vm->new_Bool( true );
        return scope->vm->new_Bool( false );
    }

    TODO;
    return scope->vm->new_Bool( true );
}

REG_PRIMITIVE_TYPE( default_construct ) {
    // copy or convert
    Type *a_type = args[ 0 ].ugs_type( scope );
    if ( args.size() == 2 && names.empty() ) {
        // if same type => copy attributes
        Type *b_type = args[ 1 ].ugs_type( scope );
        if ( a_type == b_type ) {
            for( Type::Attribute *attr = a_type->first_attribute; attr; attr = attr->next )
                Ast_visitor_Vm::init_of( scope, attr->name, args[ 1 ].find_attribute( scope, attr->name ), names );
            return scope->vm->ref_void;
        }

        // try with a converter
        if ( Variable cnv = args[ 1 ].find_attribute( scope, "convert", false ) ) {
            // if the converter returns something, it's the argument for a constructor
            if ( Variable res = cnv.apply( scope, true, args[ 0 ] ).ugs( scope ) ) {
                Vec<String> n_names;
                Vec<Variable> n_args;
                if ( res.type == scope->vm->type_Varargs ) {
                    Varargs *va = rcast( res.ptr() );
                    n_names = va->names;
                    n_args = va->values;
                } else
                    n_args << res;
                // call init_of self with args from the converter
                Ast_visitor_Vm::init_of( scope, "self", n_args, n_names );
                return scope->vm->ref_void;
            }
            // else, we assume that the constructor has already been called
            TODO;
            return scope->vm->ref_void;
        }
    }

    // given attribute values ?
    if ( names.size() && args.size() == names.size() + 1 && names.size() == a_type->attributes.size() ) {
        //        Bool_vec bv( names.size(), false );
        //        for( Type::Attribute *attr = a_type->first_attribute; attr; attr = attr->next ) {
        //            int ind = names.find( attr->name );
        //            if ( ind < 0 )
        //                return scope->add_error( "There's no attribute {} in {}, so the constructor by attribute cannot be called", attr->name, *a_type ), scope->vm->ref_void;
        //            bv.set( ind, true );
        //        }
        //        if ( ! bv.all_true() )
        //            return scope->add_error( "Some attributes of {} are not specified, so the constructor by attribute cannot be called", *a_type ), scope->vm->ref_void;

        // assign values
        for( size_t i = 0; i < names.size(); ++i ) {
            if ( a_type->attributes.count( names[ i ] ) == 0 )
                return scope->add_error( "There's no attribute {} in {} (during construction by attribute)", names[ i ], *a_type ), scope->vm->ref_void;
            Ast_visitor_Vm::init_of( scope, names[ i ], args[ 1 + i ] );
        }
        return scope->vm->ref_void;
    }

    scope->add_error( "There's no specific construct method for '{}' with args=[{}] and names=[{}]", *a_type, args, names );
    return scope->vm->ref_void;
}

REG_PRIMITIVE_TYPE( get_attributes ) {
    Type *type = args[ 0 ].ugs( scope ).apply( scope, true, {}, {}, false ).type;

    Variable res = scope->find_variable( "Vec" ).
            select( scope, true, scope->find_variable( "TypeAttribute" ) ).
            apply( scope, true ); // scope->find_variable( "Size" ).apply( scope, true, scope->vm->new_PT( type->attributes.size() ) )

    LVec<InterpTypeAttribute> &vec = *reinterpret_cast<LVec<InterpTypeAttribute> *>( res.ptr() );
    vec.resize( type->attributes.size() );
    size_t i = 0;
    for( Type::Attribute *attr = type->first_attribute; attr; attr = attr->next, ++i ) {
        vec[ i ].name   = attr->name;
        vec[ i ].offset = attr->off_in_bits;
        vec[ i ].type   = attr->type;
    }

    return res;
}

REG_PRIMITIVE_TYPE( template_attributes ) {
    Variable res = scope->find_variable( "Vec" ).
            select( scope, true, scope->find_variable( "UntypedRef" ) ).
            apply( scope, true );

    Type *type = args[ 0 ].ugs( scope ).apply( scope, true, {}, {}, false ).type;
    LVec<Variable> &vec = *reinterpret_cast<LVec<Variable> *>( res.ptr() );
    vec.resize( type->parameters.size() );
    for( size_t i = 0; i < type->parameters.size(); ++i )
        vec[ i ] = *type->parameters[ i ];

    return res;
}

REG_PRIMITIVE_FUNC( scope ) {
    Variable res( scope->vm, scope->vm->type_InterpScope );
    Scope **s = rcast( res.ptr() );
    *s = scope;
    return res;
}

REG_PRIMITIVE_TYPE( parse_met ) {
    Variable v_content  = args[ 0 ].ugs( scope );
    Variable v_filename = args[ 1 ].ugs( scope );
    String *s_content  = rcast( v_content .ptr() );
    String *s_filename = rcast( v_filename.ptr() );
    std::string content  = Rc_string( *s_content  );
    std::string filename = Rc_string( *s_filename );

    // parse
    Ast_maker am( &scope->vm->el );
    am.parse( content.c_str(), filename.c_str() );
    Ast_crepr ac = am.to_Ast_crepr();

    Variable ast_names( scope->vm, scope->vm->type_String );
    Variable ast_code ( scope->vm, scope->vm->type_String );
    reinterpret_cast<String *>( ast_names.ptr() )->init( ac.names );
    reinterpret_cast<String *>( ast_code .ptr() )->init( ac.code  );

    return args[ 2 ].ugs( scope ).apply( scope, true, { ast_names, ast_code }, { "names", "code" } );
}

REG_PRIMITIVE_FUNC( return ) {
    scope->nb_breaks = scope->nb_scopes_to_ret();
    scope->ret = scope->vm->ref_void;
    return scope->vm->ref_void;
}

REG_PRIMITIVE_FUNC( TODO ) {
    throw scope->exception( scope->vm->new_String( "TODO" ) );
}

REG_PRIMITIVE_FUNC( __arguments ) {
    for( Scope *s = scope; s; s = s->parent ) {
        if ( s->type == Scope::Scope_type::CALL ) {
            Variable res( scope->vm, scope->vm->type_Varargs );
            Varargs *va = rcast( res.ptr() );
            for( const Rc_string &name : s->__arguments_names ) {
                va->values << s->find_variable( name );
                va->names << name;
            }
            return res;
        }
    }
    return scope->add_error( "Found no caller scope" ), scope->vm->ref_void;
}

REG_PRIMITIVE_FUNC( globals ) {
    Scope *gs = &scope->vm->main_scope;
    return { scope->vm, scope->vm->type_ScopeRef, &gs };
}
