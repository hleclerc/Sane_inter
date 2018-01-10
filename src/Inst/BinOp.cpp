#include "../System/Cmp.h"
#include "../Type.h"
#include "../gvm.h"
#include "BinOp.h"
#include "Cst.h"

#include "(test_known.ts).h"

Type *type_promote( Type *ta, Type *tb ) {
    if ( ta == gvm->type_AT )
        return ta;
    return ta->content.data.type_promote_score >= tb->content.data.type_promote_score ? ta : tb;
}

Value make_Add( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va + vb ); }, Value{} ) )
        return r;

    //    // a + 0, 0 + b
    //    if ( b.is_zero() )
    //        return make_Conv( type_promote( a.type(), b.type() ), a );
    //    if ( a.is_zero() )
    //        return make_Conv( type_promote( a.type(), b.type() ), b );

    //    // ( a + k_0 ) + k_1
    //    if ( const Bin_op<Add> *add_0 = dynamic_cast<const Bin_op<Add> *>( a.inst.ptr() ) ) {
    //        if ( Value res = test_known<Value>( add_0->children[ 1 ], b, [&]( auto c, auto b ) { return make_Add( add_0->children[ 0 ], make_Value( c + b ) ); } ) )
    //            return res;
    //    }

    //    // ( a - k_0 ) + k_1
    //    if ( const Bin_op<Sub> *sub_0 = dynamic_cast<const Bin_op<Sub> *>( a.inst.ptr() ) ) {
    //        if ( Value res = test_known<Value>( sub_0->children[ 1 ], b, [&]( auto c, auto b ) { return make_Add( sub_0->children[ 0 ], make_Value( b - c ) ); } ) )
    //            return res;
    //    }

    return { new BinOp<Add>( a, b ), 0, type_promote( a.type, b.type ) };
}

Value make_Sub( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va - vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Mul( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va * vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Mod( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va % vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Div( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va / vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Div_int( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va / vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Or_bitwise( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va | vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Xor_bitwise( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va ^ vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_And_bitwise( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va & vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Shift_right( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va >> vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Shift_left( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va << vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Inf( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_inf( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Sup( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_sup( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Inf_equ( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_inf_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Sup_equ( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_sup_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Not_equ( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_not_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Equ( const Value &a, const Value &b, int flags ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}
