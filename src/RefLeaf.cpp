#include "RefLeaf.h"

RefLeaf::RefLeaf( const Value &value, Flags flags ) : value( value ), flags( flags ) {
}

void RefLeaf::write_to_stream( std::ostream &os ) const {
    os << value;
}

bool RefLeaf::is_shared() const {
    return cpt_use > 1;
}
