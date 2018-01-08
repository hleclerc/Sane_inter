#include "Reg.h"

Reg::Reg( Type *type, int num ) : type( type ), num( num ) {
}

void Reg::write_to_stream( std::ostream &os ) const {
    os << "R" << num;
}
