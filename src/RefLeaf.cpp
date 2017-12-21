#include "Inst/MemcpyKV.h"
#include "RefLeaf.h"

RefLeaf::RefLeaf( const Value &value, Flags flags ) : value( value ), flags( flags ) {
}

void RefLeaf::write_to_stream( std::ostream &os ) const {
    os << value;
}

bool RefLeaf::is_shared() const {
    return cpt_use > 1;
}

Value RefLeaf::get() const {
    return value;
}

void RefLeaf::set( const Value &src_val, SI32 dst_off ) {
    // TODO: interception
    value = make_MemcpyKV( value, src_val, dst_off );
}
