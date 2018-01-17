#pragma once

#include "../System/RcPtr.h"
#include "../System/Deque.h"
#include "CodegenData.h"
#include <functional>
#include <set>
class StreamPrio;
class StreamSep;
class Rss;
class Codegen;
class BoolVec;
class Value;
class Type;

/**
*/
class Inst : public RcObj {
public:
    struct Parent { bool operator==( const Parent &p ) const { return inst == p.inst && ninp == p.ninp; } Inst *inst; int ninp; };
    struct Inp { operator bool() const { return inst; } RcPtr<Inst> inst; int ninp; };
    using AsFunc = std::function<void(const PI8 *)>;

    Inst();
    virtual ~Inst();

    void             clear_children         ();
    void             add_child              ( const Value &ch );
    void             mod_child              ( int ninp, const Value &ch );
    void             rem_child              ( int ninp );
    void             rem_out                ( int nout, bool check_if_unused = true ); ///< shifts outputs > nout

    void             replace_by             ( int nout, Inst *new_inst, int new_nout ); ///< replace { this, nout } by { new_inst, new_nout }

    bool             all_children_with_op_id( size_t oi ) const;
    int              nb_parents_on_nout     ( int nout ) const;
    virtual void     externalize            ( Inst *inst, size_t ninp );
    virtual int      nb_outputs             () const;
    virtual int      inp_corr               ( int nout ) const;
    virtual Inp      val_corr               ( int nout ) const;
    virtual Type    *out_type               ( int nout ) const;
    virtual Inst    *clone                  () const;

    virtual void     write_to_stream        ( std::ostream &os, SI32 nout = -1, Type *type = 0, int offset = -1 ) const;
    virtual bool     write_graph_rec        ( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void(std::ostream&, const Inst *)> &f, bool disp_parents ) const;
    virtual AsFunc   get_assign_func        ( int nout, int off, int len );
    virtual void     write_dot              ( std::ostream &os ) const = 0;
    virtual void     get_bytes              ( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const;
    virtual void    *rcast                  ( SI32 nout, Type *type, SI32 offset );

    virtual void     write_inline_code      ( StreamPrio &ss, Codegen &cg, int nout, int flags ); ///< helper for case nb_outputs == 1
    virtual bool     expects_a_reg_at       ( int ninp ) const;
    virtual bool     can_be_inlined         () const;
    virtual void     write_code             ( StreamSep &ss, Codegen &cg );

    // instructions with sub graphs
    virtual Inst    *parent_out_inst        () const;
    virtual bool     simplify_for_cg        ( Codegen &cg );
    virtual void     get_out_insts          ( Deque<Inst *> &outs );

    void             thread_visitor         ( int nout, const std::function<void( Inst *inst, int nout, int ninp )> &cb, bool call_before = true );

    static void      display_graphviz       ( const Vec<Inst *> &lst, const std::function<void (std::ostream &, const Inst *)> &f = {}, const std::string &filename = ".res", bool disp_parents = false, bool launch = true );
    static void      dfs_rec                ( Inst *inst, const std::function<void(Inst*)> &f, bool deep = false, bool f_after = false, bool need_inc_ref = false );
    static void      dfs                    ( const Vec<Inst *> &lst, const std::function<void(Inst*)> &f, bool deep = false, bool f_after = false, bool need_inc_ref = false );

    size_t           creation_date; ///< used mainly for ordering during code generation
    Vec<Value>       children;
    Vec<Parent>      parents;

    CodegenData      cd;

    mutable void    *op_mp;
    mutable size_t   op_id;

    static size_t    cur_op_id;
    static size_t    date;
};

