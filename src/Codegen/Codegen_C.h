#pragma once

#include "../System/StreamPrio.h"
#include "../System/StreamSep.h"
#include "Codegen.h"
#include <map>
#include <set>

/**
*/
class Codegen_C : public Codegen {
public:
    using MTR = std::map<Type *,String>;

    Codegen_C();

    virtual void       gen_code_for  ( const Vec<Inst *> &targets ) override;
    virtual String     code          () override;

    virtual void       write_repr    ( std::ostream &os, Type *type ) override;
    virtual void       write_repr    ( std::ostream &os, const Value &value, int prio ) override;
    virtual Reg       *reg           ( Inst *inst, Type *type, int nout ) override;

    virtual void       write_fd_repr ( std::ostream &os, Type *type ) override;

protected:
    void               write_block   ( StreamSep &os, const Vec<Inst *> &out );
    void               get_scheduling( Vec<Inst *> &sched, const Vec<Inst *> &out );

    std::ostringstream declarations;
    std::ostringstream main_block;
    MTR                type_reprs;
    std::set<String>   decl_types;
    std::set<String>   decl_funcs;
    int                nb_reg;
};
