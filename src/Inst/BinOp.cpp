#include "../System/Cmp.h"
#include "../Type.h"
#include "../gvm.h"
#include "BinOp.h"
#include "Cst.h"

#include "(test_known.ts).h"

Value make_add( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va + vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_sub( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va - vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_mul( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va * vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_mod( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va % vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_div( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va / vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_div_int( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va / vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_or_bitwise( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va | vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_xor_bitwise( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va ^ vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_and_bitwise( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va & vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_shift_right( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va >> vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_shift_left( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( va << vb ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_inf( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_inf( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_sup( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_sup( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_inf_equ( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_inf_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_sup_equ( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_sup_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_not_equ( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_not_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_equ( const Value &a, const Value &b ) {
    if ( Value r = test_known( a, b, []( auto va, auto vb ) { return make_Cst( Cmp<Signed<decltype(va)>::val,Signed<decltype(vb)>::val>::my_equ( va, vb ) ); }, Value{} ) )
        return r;
    TODO; return {};
}
