#include "../Codegen/Codegen.h"
#include "MemcpyKV.h"
#include "../Type.h"

MemcpyKV::MemcpyKV( const Value &dst, const Value &src, SI32 dst_off ) : dst_off( dst_off ) {
    ASSERT( dst.offset == 0, "..." );

    add_child( dst );
    add_child( src );
}

int MemcpyKV::nb_outputs() const {
    return 1;
}

void MemcpyKV::write_code( StreamSep &ss, Codegen &cg ) {
    if ( cd.regs.empty() )
        cd.regs << children[ 0 ].inst->cd.regs[ children[ 0 ].nout ];

    if ( write_ssp_rec( ss, cg, dst_off, children[ 0 ].type, "" ) )
        return;
    ss << "memcpy_bits(...);";
}

void MemcpyKV::write_dot( std::ostream &os ) const {
    os << "MemcpyKV[" << dst_off << "]";
}

void MemcpyKV::get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    TODO;
}

int MemcpyKV::inp_corr( int nout ) const {
    return nout == 0 ? 0 : -1;
}

bool MemcpyKV::write_ssp_rec( StreamSep &ss, Codegen &cg, int dst_offset, Type *dst_type, String m ) const {
    if ( dst_offset == 0 && dst_type == children[ 1 ].type ) {
        ss << cg.repr( children[ 0 ], PRIO_Member_selection ) << m << " = " << cg.repr( children[ 1 ], PRIO_Assignment ) << ";";
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
