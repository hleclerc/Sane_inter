#pragma once

#include "../Value.h"

/**
*/
template<class Op>
class BinOp : public Inst {
public:
    BinOp( const Value &a, const Value &b ){
        add_child( a );
        add_child( b );
    }

    virtual void write_dot( std::ostream &os ) const override {
        os << "pouet";
    }
};

Value make_add        ( const Value &a, const Value &b );
Value make_sub        ( const Value &a, const Value &b );
Value make_mul        ( const Value &a, const Value &b );
Value make_mod        ( const Value &a, const Value &b );
Value make_div        ( const Value &a, const Value &b );
Value make_div_int    ( const Value &a, const Value &b );
Value make_or_bitwise ( const Value &a, const Value &b );
Value make_xor_bitwise( const Value &a, const Value &b );
Value make_and_bitwise( const Value &a, const Value &b );
Value make_shift_right( const Value &a, const Value &b );
Value make_shift_left ( const Value &a, const Value &b );
Value make_inf        ( const Value &a, const Value &b );
Value make_sup        ( const Value &a, const Value &b );
Value make_inf_equ    ( const Value &a, const Value &b );
Value make_sup_equ    ( const Value &a, const Value &b );
Value make_not_equ    ( const Value &a, const Value &b );
Value make_equ        ( const Value &a, const Value &b );
