#pragma once

#include "../Codegen/Codegen.h"
#include "Clonable.h"
#include "../gvm.h"

/**
*/
template<class Op>
class BinOp : public Clonable<BinOp<Op>> {
public:
    BinOp( const Value &a, const Value &b ){
        this->add_child( a );
        this->add_child( b );
    }
    BinOp( AttrClone, const BinOp *orig ) {
    }

    virtual void write_dot( std::ostream &os ) const override {
        os << Op::name();
    }

    virtual int nb_outputs() const override {
        return 1;
    }

    virtual Type *out_type( int nout ) const override {
        return Op::type( this->children[ 0 ].type, this->children[ 1 ].type );
    }

    virtual void write_inline_code( StreamPrio &ss, Codegen &cg, int flags ) override {
        ss( Op::prio ) << cg.repr( this->children[ 0 ], Op::prio ) << " " << Op::str() << " " << cg.repr( this->children[ 1 ], Op::prio );
    }
};

Type *type_promote( Type *ta, Type *tb );

#define DECL_BIN_OP( NAME, PRIO, STR, TYPE ) \
    Value make_##NAME( const Value &a, const Value &b, int flags = 0 ); \
    struct NAME { \
        enum { prio = PRIO }; \
        static const char *str () { return STR; } \
        static const char *name() { return #NAME; } \
        static Value       make( const Value &a, const Value &b ) { return make_##NAME( a, b ); } \
        static Type       *type( Type *a, Type *b ) { return TYPE; } \
    }

DECL_BIN_OP( Add        , PRIO_Addition             , "+" , type_promote( a, b ) );
DECL_BIN_OP( Sub        , PRIO_Subtraction          , "-" , type_promote( a, b ) );
DECL_BIN_OP( Mul        , PRIO_Multiplication       , "*" , type_promote( a, b ) );
DECL_BIN_OP( Div        , PRIO_Division             , "/" , type_promote( a, b ) );
DECL_BIN_OP( Div_int    , PRIO_Division             , "/" , type_promote( a, b ) );
DECL_BIN_OP( Mod        , PRIO_Modulus              , "%" , type_promote( a, b ) );
DECL_BIN_OP( Inf        , PRIO_Less_than            , "<" , gvm->type_Bool       );
DECL_BIN_OP( Sup        , PRIO_Less_than            , ">" , gvm->type_Bool       );
DECL_BIN_OP( Inf_equ    , PRIO_Less_than_or_equal_to, "<=", gvm->type_Bool       );
DECL_BIN_OP( Sup_equ    , PRIO_Less_than_or_equal_to, ">=", gvm->type_Bool       );
DECL_BIN_OP( Equ        , PRIO_Equality             , "==", gvm->type_Bool       );
DECL_BIN_OP( Not_equ    , PRIO_Inequality           , "!=", gvm->type_Bool       );
DECL_BIN_OP( Or_logical , PRIO_Logical_OR           , "||", gvm->type_Bool       );
DECL_BIN_OP( And_logical, PRIO_Logical_AND          , "&&", gvm->type_Bool       );
DECL_BIN_OP( Or_bitwise , PRIO_Bitwise_inclusive_OR , "|" , type_promote( a, b ) );
DECL_BIN_OP( Xor_bitwise, PRIO_Bitwise_exclusive_OR , "^" , type_promote( a, b ) );
DECL_BIN_OP( And_bitwise, PRIO_Bitwise_AND          , "&" , type_promote( a, b ) );
DECL_BIN_OP( Shift_right, PRIO_Right_shift          , "<<", a                    );
DECL_BIN_OP( Shift_left , PRIO_Left_shift           , ">>", a                    );
