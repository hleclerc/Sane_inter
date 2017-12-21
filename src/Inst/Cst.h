#pragma once

#include "../System/BoolVec.h"
#include "../Value.h"

/**
*/
class Cst : public Inst {
public:
    template<class T>
    static Value make() {
        return {};
    }

    Cst( int size, void *val = 0, void *kno = 0 );

    virtual void write_dot( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const override;
    virtual void get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const override;

    BoolVec val;
    BoolVec kno;
};


Value make_Cst_PI64( PI64 val );
Value make_Cst_SI64( SI64 val );
Value make_Cst_PI32( PI32 val );
Value make_Cst_SI32( SI32 val );
Value make_Cst_PI16( PI16 val );
Value make_Cst_SI16( SI16 val );
Value make_Cst_PI8 ( PI8  val );
Value make_Cst_SI8 ( SI8  val );
Value make_Cst_Bool( Bool val );

inline Value make_Cst( PI64 val ) { return make_Cst_PI64( val ); }
inline Value make_Cst( SI64 val ) { return make_Cst_SI64( val ); }
inline Value make_Cst( PI32 val ) { return make_Cst_PI32( val ); }
inline Value make_Cst( SI32 val ) { return make_Cst_SI32( val ); }
inline Value make_Cst( PI16 val ) { return make_Cst_PI16( val ); }
inline Value make_Cst( SI16 val ) { return make_Cst_SI16( val ); }
inline Value make_Cst( PI8  val ) { return make_Cst_PI8 ( val ); }
inline Value make_Cst( SI8  val ) { return make_Cst_SI8 ( val ); }
inline Value make_Cst( Bool val ) { return make_Cst_Bool( val ); }

Value make_Cst( Type *type );
