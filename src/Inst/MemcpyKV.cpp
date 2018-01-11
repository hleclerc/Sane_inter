#include "../Codegen/Codegen.h"
#include "MemcpyKV.h"
#include "../Type.h"

MemcpyKV::MemcpyKV( const Value &dst, const Value &src, SI32 dst_off ) : dst_off( dst_off ) {
    ASSERT( dst.offset == 0, "..." );

    add_child( dst );
    add_child( src );
}

bool MemcpyKV::expects_a_reg_at( int ninp ) const {
    return ninp == 0;
}

bool MemcpyKV::can_be_inlined() const {
    return false;
}

int MemcpyKV::nb_outputs() const {
    return 1;
}

void MemcpyKV::write_code( StreamSep &ss, Codegen &cg ) {
    cd.out_regs = { children[ 0 ].inst->cd.out_regs[ children[ 0 ].nout ] };

    if ( write_ssp_rec( ss, cg, dst_off, children[ 0 ].type, "" ) )
        return;
    ss << "memcpy_bits(...);";
}

void MemcpyKV::write_dot( std::ostream &os ) const {
    os << "MemcpyKV[" << dst_off << "]";
}

void MemcpyKV::get_bytes( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const {
    // we have to get data from...
    int len_ssp = children[ 1 ].type->size(), end_ssp = dst_off + len_ssp, end_src = beg_src + len;
    if ( beg_src < dst_off ) { // left part of children[ 0 ] and...
        if ( end_src <= dst_off ) { // => only children[ 0 ]
            children[ 0 ].inst->get_bytes( children[ 0 ].nout, dst, beg_dst, beg_src, len, msk );
        } else if ( end_src <= end_ssp ) {
            children[ 1 ].inst->get_bytes( children[ 1 ].nout, dst, beg_dst + dst_off - beg_src, 0, end_src - dst_off, msk );
            children[ 0 ].inst->get_bytes( children[ 0 ].nout, dst, beg_dst, beg_src, len, msk );
        } else {
            children[ 1 ].inst->get_bytes( children[ 1 ].nout, dst, beg_dst + dst_off - beg_src, 0, len_ssp, msk );
            children[ 0 ].inst->get_bytes( children[ 0 ].nout, dst, beg_dst, beg_src, len, msk );
        }
    } else if ( beg_src < end_ssp ) { // children[ 1 ] and...
        if ( end_src <= end_ssp ) {
            children[ 1 ].inst->get_bytes( children[ 1 ].nout, dst, beg_dst, beg_src - dst_off, len, msk );
        } else {
            children[ 1 ].inst->get_bytes( children[ 1 ].nout, dst, beg_dst, beg_src - dst_off, end_ssp - beg_src, msk );
            children[ 0 ].inst->get_bytes( children[ 0 ].nout, dst, beg_dst, beg_src, len, msk );
        }
    } else { // right part of children[ 0 ]
        children[ 0 ].inst->get_bytes( children[ 0 ].nout, dst, beg_dst, beg_src, len, msk );
    }
}

int MemcpyKV::inp_corr( int nout ) const {
    return nout == 0 ? 0 : -1;
}

bool MemcpyKV::write_ssp_rec( StreamSep &ss, Codegen &cg, int dst_offset, Type *dst_type, String m ) const {
    if ( dst_offset == 0 && dst_type == children[ 1 ].type ) {
        ss << cg.repr( children[ 0 ], PRIO_Member_selection ) << m << " = " << cg.repr( children[ 1 ], PRIO_Assignment, Codegen::WriteInlineCodeFlags::type_is_forced ) << ";";
        return true;
    }

    for( const TypeContent::Attribute *attr = dst_type->content.data.first_attribute; attr; attr = attr->next ) {
        if ( dst_offset < attr->off )
            return false;
        if ( write_ssp_rec( ss, cg, dst_offset - attr->off, attr->type, m + "." + attr->name ) )
            return true;
    }
    return false;
}

Value make_MemcpyKV( const Value &dst, const Value &src, SI32 dst_off ) {
    return { new MemcpyKV( dst, src, dst_off ), 0, dst.type };
}
