#include "RefLeaf.h"

RefLeaf::RefLeaf(const Value &value ) : value( value ) {
}

void RefLeaf::write_to_stream( std::ostream &os ) const {
    os << value;
}
