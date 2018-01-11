#pragma once

#include "../System/Stream.h"
class Reg;

/**
*/
struct CodegenData {
    CodegenData() {}

    size_t      num_in_sched;
    Vec<Reg *>  out_regs;
};
