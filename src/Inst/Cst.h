#pragma once

#include "../System/BoolVec.h"
#include "Clonable.h"

/**
*/
class Cst : public Clonable<Cst,Inst> {
public:
    Cst( Type *type, int size, void *val = 0, void *kno = 0 );
    Cst( AttrClone, const Cst *cst );

    virtual void  write_inline_code( StreamPrio &ss, Codegen &cg ) override; ///<
    virtual void  write_to_stream  ( std::ostream &os, SI32 nout = -1, Type *type = 0, int offset = -1 ) const;
    virtual int   nb_outputs       () const override;
    virtual void  write_dot        ( std::ostream &os ) const override;
    virtual void  get_bytes        ( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const override;
    virtual Type *out_type         ( int nout ) const;

    Type   *type;
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
