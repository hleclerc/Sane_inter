#pragma once

#include "CompartmentalisationData.h"
#include "../System/StreamPrio.h"
#include "../System/StreamSep.h"
#include "../System/Deque.h"
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
    struct ReprType  { void write_to_stream( std::ostream &os ) const { cg->write_repr( os, type               ); } Codegen *cg; Type        *type;                       };
    struct ReprValue { void write_to_stream( std::ostream &os ) const { cg->write_repr( os, value, prio, flags ); } Codegen *cg; const Value &value; int prio; int flags; };
    struct WriteInlineCodeFlags { enum { type_is_forced = 1 }; };

    virtual void     gen_code_for                  ( const Vec<Inst *> &targets ) = 0;
    virtual String   code                          () = 0;

    ReprType         repr                          ( Type *type );
    ReprValue        repr                          ( const Value &val, int prio = PRIO_paren, int flags = 0 );

    virtual void     write_block                   ( StreamSep &os, const Vec<Inst *> &out ) = 0;
    virtual void     write_repr                    ( std::ostream &os, Type *type ) = 0;
    virtual void     write_repr                    ( std::ostream &os, const Value &value, int prio, int flags ) = 0;

    virtual String   write_func_write_fd           ( Type *type ) = 0;

    virtual Reg     *new_reg_for                   ( Inst *inst, Type *type, int nout ) = 0;

    virtual Codegen *new_child                     () = 0;

    void             base_simplifications          ( Inst *inst );

private:
    bool             base_simplifications_rec      ( Inst *inst );

    void             compartmentalisation          ( const Vec<Inst *> &lst );
    void             init_CompartmentalisationData ( Deque<CompartmentalisationData> &cv, Deque<Inst *> &outs, Inst *inst, Inst *cur_out, size_t beg_op_id );
    void             split_CompartmentalisationData( Inst *inst, Inst *out_inst, size_t init_op_id );
    void             remove_rec_outs               ( Inst *inst, Inst *out );
};
