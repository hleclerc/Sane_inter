#pragma once

#include "Clonable.h"

/**
*/
class MemcpyKV : public Clonable<MemcpyKV,Inst> {
public:
    MemcpyKV( const Value &dst, const Value &src, SI32 dst_off );
    MemcpyKV( AttrClone, const MemcpyKV *a ) { dst_off = a->dst_off; }

    virtual int  nb_outputs   () const override;
    virtual void write_code   ( StreamSep &ss, Codegen &cg ) override;
    virtual void write_dot    ( std::ostream &os ) const override;
    virtual void get_bytes    ( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const override;
    virtual int  inp_corr     ( int nout ) const override;

    bool         write_ssp_rec( StreamSep &ss, Codegen &cg, int dst_offset, Type *dst_type, String m ) const;


    SI32         dst_off;
};

Value make_MemcpyKV( const Value &dst, const Value &src, SI32 dst_off );
