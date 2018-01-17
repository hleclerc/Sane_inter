#pragma once

#include "Clonable.h"
class While;

/***/
class WhileInp : public Clonable<WhileInp,Inst> {
public:
    WhileInp( While *while_inst );
    WhileInp( AttrClone, const WhileInp *a );

    // virtual bool   get_cpt_ref( int nout, Ref_count &rc ) const override;
    virtual int    nb_outputs () const override;
    virtual void   write_code ( StreamSep &ss, Codegen &cg ) override;
    virtual void   write_dot  ( std::ostream &os ) const override;
    // virtual Inp    val_corr   ( int nout ) const override;
    // virtual Inst  *dcast      ( int nout ) override;
    // virtual Type  *type       ( int nout ) const override;

    While         *while_inst;
};

/***/
class WhileOut : public Clonable<WhileOut,Inst> {
public:
    WhileOut( While *while_inst, WhileInp *inp_inst );
    WhileOut( AttrClone, const WhileOut *a );

    virtual Inst *parent_out_inst() const override;
    virtual void  externalize    ( Inst *inst, size_t ninp ) override;
    // virtual bool  get_cpt_ref    ( int nout, Ref_count &rc ) const override;
    virtual int   nb_outputs     () const override;
    virtual void  write_code     ( StreamSep &ss, Codegen &cg ) override;
    virtual void  write_dot      ( std::ostream &os ) const override;

    While        *while_inst;
    WhileInp     *inp_inst;
};

/***/
class While : public Clonable<While,Inst> {
public:
    While();
    While( AttrClone, const While *a );

    virtual bool    simplify_for_cg( Codegen &cg ) override;
    virtual bool    write_graph_rec( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void(std::ostream&,const Inst *)> &f, bool disp_parents ) const override;
    virtual void    get_out_insts  ( Deque<Inst *> &outs ) override;
    // virtual bool    get_cpt_ref    ( int nout, Ref_count &rc ) const override; ///< return false if cpt_ref is not decidable
    void            attr_clone     ( const While *a );
    virtual int     nb_outputs     () const override;
    virtual void    write_code     ( StreamSep &ss, Codegen &cg ) override;
    virtual void    write_dot      ( std::ostream &os ) const override;
    // virtual Inst   *dcast          ( int nout ) override;
    // virtual Type   *type           ( int nout ) const override;

    RcPtr<WhileInp> inp_inst;
    RcPtr<WhileOut> out_inst;
};
