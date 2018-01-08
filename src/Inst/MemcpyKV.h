#pragma once

#include "../Value.h"

/**
*/
class MemcpyKV : public Inst {
public:
    MemcpyKV( const Value &dst, const Value &src, SI32 dst_off );

    virtual int  nb_outputs() const override;
    virtual void write_dot ( std::ostream &os ) const override;
    virtual void get_bytes ( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const override;
    virtual int  inp_corr  ( int nout ) const override;

    SI32         dst_off;
};

Value make_MemcpyKV( const Value &dst, const Value &src, SI32 dst_off );
