#include "../Value.h"
#include "Codegen.h"

Codegen::ReprType Codegen::repr( Type *type ) {
    return { this, type };
}

Codegen::ReprValue Codegen::repr( const Value &val, int prio, int flags ) {
    return { this, val, prio, flags };
}

void Codegen::base_simplifications( Inst *inst ) {
    compartmentalisation( inst );

    while( true ) {
        ++Inst::cur_op_id;
        if ( ! base_simplifications_rec( inst ) )
            break;
    }
}

bool Codegen::base_simplifications_rec( Inst *inst ) {
    // out insts
    Deque<Inst *> outs;
    inst->get_out_insts( outs );
    bool change = false;
    for( Inst *out : outs )
        change |= base_simplifications_rec( out );
    if ( change )
        return true;

    // children
    for( const Value &ch : inst->children )
        if ( base_simplifications_rec( ch.inst.ptr() ) )
            return true;

    return inst->simplify_for_cg( *this );
}

void Codegen::compartmentalisation(const Vec<Inst *> &lst) {
    // init inst->cd.cmd.outs
    size_t beg_op_id = ++Inst::cur_op_id;
    Deque<CompartmentalisationData> cv;
    Deque<Inst *> outs;
    for( Inst *inst : lst )
        init_CompartmentalisationData( cv, outs, inst, 0, beg_op_id );
    for( size_t i = 0; i < outs.size(); ++i ) { // size changes during the process
        ++Inst::cur_op_id;
        init_CompartmentalisationData( cv, outs, outs[ i ], outs[ i ], beg_op_id );
    }

    // split
    for( size_t i = outs.size(); i--; ) {
        ++Inst::cur_op_id;
        split_CompartmentalisationData( outs[ i ], outs[ i ], beg_op_id );
    }
}

void Codegen::init_CompartmentalisationData(Deque<CompartmentalisationData> &cv, Deque<Inst *> &outs, Inst *inst, Inst *cur_out, size_t beg_op_id) {
    if ( inst->op_id < beg_op_id )
        inst->op_mp = cv.new_back_item();

    if ( inst->op_id == Inst::cur_op_id )
        return;
    inst->op_id = Inst::cur_op_id;

    CMD( inst )->outs << cur_out;

    size_t old_outs_size = outs.size();
    inst->get_out_insts( outs );

    // set parent_out for each new sub out
    for( ; old_outs_size < outs.size(); ++old_outs_size ) {
        Inst *new_out = outs[ old_outs_size ];
        if ( new_out->op_id < beg_op_id ) {
            new_out->op_id = beg_op_id;
            new_out->op_mp = cv.new_back_item();
        }
        CMD( outs[ old_outs_size ] )->parent_out = cur_out;
    }

    // rec
    for( size_t ninp = 0; ninp < inst->children.size(); ++ninp ) {
        init_CompartmentalisationData( cv, outs, inst->children[ ninp ].inst.ptr(), cur_out, beg_op_id );
        CMD( inst->children[ ninp ].inst )->parents << CompartmentalisationData::P{ inst, ninp };
    }
}

void Codegen::split_CompartmentalisationData(Inst *inst, Inst *out_inst, size_t init_op_id) {
    if ( inst->op_id == Inst::cur_op_id )
        return;
    inst->op_id = Inst::cur_op_id;

    for( size_t ninp = 0; ninp < inst->children.size(); ++ninp ) {
        Value &ch = inst->children[ ninp ];
        if ( ch.inst->op_id >= init_op_id && ! CMD( ch.inst )->outs.only_has( out_inst ) ) {
            remove_rec_outs( ch.inst.ptr(), out_inst );
            out_inst->externalize( inst, ninp );
        } else
            split_CompartmentalisationData( ch.inst.ptr(), out_inst, init_op_id );
    }
}

void Codegen::remove_rec_outs( Inst *inst, Inst *out ) {
    int ind = CMD( inst )->outs.index_first( out );
    if ( ind >= 0 ) {
        CMD( inst )->outs.remove_unordered( ind );
        for( Inst *p : CMD( out->parent_out_inst() )->outs )
            CMD( inst )->outs.push_back_unique( p );

        for( const Value &ch : inst->children )
            remove_rec_outs( ch.inst.ptr(), out );
    }
}
