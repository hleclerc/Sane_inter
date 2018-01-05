#pragma once

#include "../System/RcPtr.h"
#include <set>
class BoolVec;
class Value;
class Type;

/**
*/
class Inst : public RcObj {
public:
    struct Inp { operator bool() const { return inst; } RcPtr<Inst> inst; size_t ninp; };

    virtual ~Inst();

    void             add_child       ( const Value &ch );
    void             add_dep         ( const RcPtr<Inst> &inst );

    virtual int      nb_outputs             () const;
    virtual int      inp_corr               ( int nout ) const;
    virtual Inp      val_corr               ( int nout ) const;
    virtual Inst    *clone                  () const;

    virtual void     write_to_stream ( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const;
    virtual bool     write_graph_rec ( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void(std::ostream&, const Inst *)> &f, bool disp_parents ) const;
    virtual void     write_dot       ( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const = 0;
    virtual void     get_bytes       ( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const;
    virtual void    *rcast           ( SI32 nout, Type *type, SI32 offset );

    static  void     display_graphviz( const Vec<Inst *> &lst, const std::function<void (std::ostream &, const Inst *)> &f = {}, const std::string &filename = ".res", bool disp_parents = false, bool launch = true );

    Vec<Value>       children;
    Vec<RcPtr<Inst>> deps;
};

