#pragma once

#include "../System/StreamPrio.h"
#include "../System/StreamSep.h"
#include "../System/Vec.h"
#include "Prio.h"
#include "Reg.h"
class Value;
class Type;
class Inst;

/**
*/
class Codegen {
public:
    struct ReprType  { void write_to_stream( std::ostream &os ) const { cg->write_repr( os, type        ); } Codegen *cg; Type        *type;            };
    struct ReprValue { void write_to_stream( std::ostream &os ) const { cg->write_repr( os, value, prio ); } Codegen *cg; const Value &value; int prio; };

    virtual void   gen_code_for ( const Vec<Inst *> &targets ) = 0;
    virtual String code         () = 0;

    ReprType       repr         ( Type *type );
    ReprValue      repr         ( const Value &val, int prio = PRIO_paren );

    virtual void   write_repr   ( std::ostream &os, Type *type ) = 0;
    virtual void   write_repr   ( std::ostream &os, const Value &value, int prio ) = 0;

    virtual void   write_fd_repr( std::ostream &os, Type *type ) = 0;

    virtual Reg   *reg          ( Inst *inst, Type *type, int nout ) = 0;
};