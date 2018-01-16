#pragma once

#include "../System/BoolVec.h"
class Inst;

/**
*/
struct CompartmentalisationData {
    struct P {
        Inst  *inst;
        size_t ninp;
    };

    Vec<Inst *> outs;
    Vec<P>      parents;
    Inst       *parent_out;
};

#define CMD( INST ) reinterpret_cast<CompartmentalisationData *>( ( INST )->op_mp )
