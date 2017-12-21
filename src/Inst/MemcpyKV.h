#pragma once

#include "../Value.h"

/**
*/
class MemcpyKV : public Inst {
public:
    MemcpyKV( const Value &dst, const Value &src, SI32 dst_off );

    virtual void write_dot( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const override;
    virtual void get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const override;

    SI32         dst_off;
};

Value make_MemcpyKV( const Value &dst, const Value &src, SI32 dst_off );
