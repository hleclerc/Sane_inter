#include "../Value.h"
#include "Codegen.h"

Codegen::ReprType Codegen::repr( Type *type ) {
    return { this, type };
}

Codegen::ReprValue Codegen::repr( const Value &val, int prio, int flags ) {
    return { this, val, prio, flags };
}

void Codegen::base_simplifications( Inst *inst ) {
    while( true ) {
        ++Inst::cur_op_id;
        if ( ! base_simplifications_rec( inst ) )
            break;
    }
}

bool Codegen::base_simplifications_rec( Inst *inst ) {
    for( const Value &ch : inst->children )
        if ( base_simplifications_rec( ch.inst.ptr() ) )
            return true;
    for( const RcPtr<Inst> &ch : inst->deps )
        if ( base_simplifications_rec( ch.ptr() ) )
            return true;

    return inst->simplify_for_cg( *this );
}
