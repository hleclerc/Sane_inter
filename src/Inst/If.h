#pragma once

#include "Clonable.h"
class If;

/***/
class IfInp : public Clonable<IfInp,Inst> {
public:
    IfInp() {}
    IfInp( AttrClone, const IfInp *a ) {}

    // virtual bool   get_cpt_ref( int nout, Ref_count &rc ) const override;
    virtual int    nb_outputs () const override;
    virtual void   write_code ( StreamSep &ss, Codegen &cg ) override;
    virtual void   write_dot  ( std::ostream &os ) const override;
    virtual Inp    val_corr   ( int nout ) const override;
    // virtual Inst  *dcast      ( int nout ) override;
    virtual Type  *out_type   ( int nout ) const override;

    //    virtual void  find_usable_ref_rec( int nout, std::function<void(Ptr_on_ref *)> f, int sg ) override;

    If            *if_inst;
};

/***/
class IfOut : public Clonable<IfOut,Inst> {
public:
    IfOut( const Vec<Value> &out );
    IfOut( AttrClone, const IfOut *a ) {}

    virtual Inst *parent_out_inst() const override;
    virtual void  write_code     ( StreamSep &ss, Codegen &cg ) override;
    virtual int   nb_outputs     () const override;
    virtual void  write_dot      ( std::ostream &os ) const override;

    IfInp        *if_inp;
    If           *if_inst;
};

/**
  condition is the first variable
*/
class If : public Clonable<If,Inst> {
public:
    If( const Vec<Value> &inp, RcPtr<IfInp> inp_ok, IfOut *out_ok, RcPtr<IfInp> inp_ko, IfOut *out_ko );
    If( AttrClone, const If *a );

    virtual bool  expects_a_reg_at    ( int ninp ) const override;
    virtual bool  simplify_for_cg     ( Codegen &cg ) override;
    virtual bool  write_graph_rec     ( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void(std::ostream&, const Inst *)> &f, bool disp_parents ) const override;
    virtual bool  can_be_inlined      () const override;
    // virtual void  virtual_destroy     ( int nout ) const override;
    virtual void  get_out_insts       ( Deque<Inst *> &outs ) override;
    // virtual bool  get_cpt_ref         ( int nout, Ref_count &rc ) const override; ///< return false if cpt_ref is not decidable
    void          attr_clone          ( const If *a );
    virtual int   nb_outputs          () const override;
    virtual void  write_code          ( StreamSep &ss, Codegen &cg ) override;
    virtual void  write_dot           ( std::ostream &os ) const override;
    //    virtual void  p_dec_ref           ( int nout, Scope *scope ) override;
    //    virtual void  p_inc_ref           ( int nout, Scope *scope ) override;

    virtual int   inp_corr            ( int nout ) const override;
    //    virtual Type *p_type              ( int nout, Scope *scope ) const override;
    //    virtual Value p_get               ( int nout, Scope *scope ) const override;
    //    virtual void  p_set               ( int nout, Scope *scope, const Value &new_val, const Value &offset, int cst = 0 ) override;
    //    virtual Inst *dcast               ( int nout ) override;
    //    virtual Type *type                ( int nout ) const override;
    //    virtual Value size                ( int nout ) const override;
    //    virtual Value alig                ( int nout ) const override;

    RcPtr<IfInp>  inp_ok;
    RcPtr<IfOut>  out_ok;
    RcPtr<IfInp>  inp_ko;
    RcPtr<IfOut>  out_ko;
};

Value new_If( const Vec<Value> &inp, IfInp *inp_ok, IfOut *out_ok, IfInp *inp_ko, IfOut *out_ko, int nout = 0 );
