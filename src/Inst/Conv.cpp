#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "Conv.h"

Conv::Conv( const Value &orig, Type *target_type ) : target_type( target_type ) {
    add_child( orig );
}

Conv::Conv( AttrClone, const Conv *orig ) : target_type( orig->target_type ) {
}

void Conv::write_dot( std::ostream &os ) const {
    os << "Conv[" << *target_type << "]";
}

int Conv::nb_outputs() const {
    return 1;
}

Type *Conv::out_type( int nout ) const {
    return target_type;
}

void Conv::write_inline_code( StreamPrio &ss, Codegen &cg, int nout, int flags ) {
    if ( flags & Codegen::WriteInlineCodeFlags::type_is_forced )
        ss << cg.repr( this->children[ 0 ], ss.prio, flags );
    else
        ss( PRIO_Function_call ) << cg.repr( target_type ) << "(" << cg.repr( this->children[ 0 ], PRIO_Cast, Codegen::WriteInlineCodeFlags::type_is_forced ) << ")";
}

Value make_Conv( const Value &orig, Type *target_type ) {
    return { new Conv( orig, target_type ), 0, target_type };
}
