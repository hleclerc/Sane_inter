#pragma once

#include "../System/RcPtr.h"
#include "../System/Deque.h"
#include "CodegenData.h"
#include <set>
class StreamPrio;
class StreamSep;
class Codegen;
class BoolVec;
class Value;
class Type;

/**
*/
class Inst : public RcObj {
public:
    struct Parent { bool operator==( const Parent &p ) const { return inst == p.inst && ninp == p.ninp; } Inst *inst; ssize_t ninp; };
    struct Inp { operator bool() const { return inst; } RcPtr<Inst> inst; size_t ninp; };

    Inst();
    virtual ~Inst();

    void             add_child              ( const Value &ch );
    void             add_dep                ( const RcPtr<Inst> &inst );

    bool             all_children_with_op_id( size_t oi ) const;
    virtual int      nb_outputs             () const;
    virtual int      inp_corr               ( int nout ) const;
    virtual Inp      val_corr               ( int nout ) const;
    virtual Inst    *clone                  () const;

    virtual void     write_to_stream        ( std::ostream &os, SI32 nout = -1, Type *type = 0, int offset = -1 ) const;
    virtual bool     write_graph_rec        ( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void(std::ostream&, const Inst *)> &f, bool disp_parents ) const;
    virtual void     write_dot              ( std::ostream &os ) const = 0;
    virtual void     get_bytes              ( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const;
    virtual void    *rcast                  ( SI32 nout, Type *type, SI32 offset );

    virtual void     write_inline_code      ( StreamPrio &ss, Codegen &cg, int nout, Type *type, int offset ); ///<
    virtual void     write_code             ( StreamSep &ss, Codegen &cg );

    // instructions with sub graphs
    virtual Inst    *parent_out_inst        () const;
    virtual void     get_out_insts          ( Deque<Inst *> &outs );
    virtual void     externalize            ( Inst *inst, size_t ninp );

    static void      display_graphviz       ( const Vec<Inst *> &lst, const std::function<void (std::ostream &, const Inst *)> &f = {}, const std::string &filename = ".res", bool disp_parents = false, bool launch = true );
    static void      dfs_rec                ( Inst *inst, const std::function<void(Inst*)> &f, bool deep = false, bool f_after = false, bool need_inc_ref = false );
    static void      dfs                    ( const Vec<Inst *> &lst, const std::function<void(Inst*)> &f, bool deep = false, bool f_after = false, bool need_inc_ref = false );

    size_t           creation_date; ///< used mainly for ordering during code generation
    Vec<Value>       children;
    Vec<Parent>      parents;
    Vec<RcPtr<Inst>> deps;

    CodegenData      cd;

    mutable void    *op_mp;
    mutable size_t   op_id;

    static size_t    cur_op_id;
    static size_t    date;
};

