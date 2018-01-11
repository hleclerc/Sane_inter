#pragma once

#include "../Codegen/Codegen.h"
#include "Clonable.h"
#include "../gvm.h"

/**
*/
template<class Op>
class UnaOp : public Clonable<UnaOp<Op>> {
public:
    UnaOp( const Value &a ){
        this->add_child( a );
    }
    UnaOp( AttrClone, const UnaOp *orig ) {
    }

    virtual void write_dot( std::ostream &os ) const override {
        os << Op::name();
    }

    virtual int nb_outputs() const override {
        return 1;
    }

    virtual Type *out_type( int nout ) const override {
        return Op::type( this->children[ 0 ].type );
    }

    virtual void write_inline_code( StreamPrio &ss, Codegen &cg, int nout, int flags ) override {
        ss( Op::prio ) << Op::str() << " " << cg.repr( this->children[ 0 ], Op::prio );
    }
};

#define DECL_UNA_OP( NAME, PRIO, STR, TYPE ) \
    Value make_##NAME( const Value &a ); \
    struct NAME { \
        enum { prio = PRIO }; \
        static const char *str () { return STR; } \
        static const char *name() { return #NAME; } \
        static Value       make( const Value &a ) { return make_##NAME( a ); } \
        static Type       *type( Type *a ) { return TYPE; } \
    }

DECL_UNA_OP( Neg        , PRIO_Unary_negation, "-", a );
DECL_UNA_OP( Not_logical, PRIO_Logical_not   , "!", gvm->type_Bool );
DECL_UNA_OP( Not_bitwise, PRIO_Logical_not   , "~", a );
