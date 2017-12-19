#include "FunctionSignature.h"

FunctionSignature::FunctionSignature( Def *def ) : name( def->name ) {
}

void FunctionSignature::write_to_stream( std::ostream &os ) const {
    os << name;
}
