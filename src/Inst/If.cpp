#include "../Codegen/Codegen.h"
#include "../Type.h"
// #include "../Ref_count.h"
#include "UnaOp.h"
//#include "Room.h"
//#include "Copy.h"
#include "If.h"

//bool IfInp::get_cpt_ref( int nout, Ref_count &rc ) const {
//    return true;
//}

int IfInp::nb_outputs() const {
    return if_inst->children.size();
}

void IfInp::write_dot( std::ostream &os ) const {
    os << "IfInp";
}

Inst::Inp IfInp::val_corr( int nout ) const {
    return Inp{ if_inst, nout };
}

void IfInp::write_code( StreamSep &ss, Codegen &cg ) {
    for( size_t ninp = 0; ninp < if_inst->children.size(); ++ninp )
        cd.out_regs.secure_set( ninp, if_inst->children[ ninp ].inst->cd.out_regs.secure_get( if_inst->children[ ninp ].nout, 0 ) );
}

Type *IfInp::out_type( int nout ) const {
    return if_inst->children[ nout ].type;
}

IfOut::IfOut( const Vec<Value> &out ) {
    for( const Value &ch : out )
        add_child( ch );
}

void IfOut::write_dot( std::ostream &os ) const {
    os << "IfOut";
}

int IfOut::nb_outputs() const {
    return 0;
}

void IfOut::write_code( StreamSep &ss, Codegen &cg ) {
}

Inst *IfOut::parent_out_inst() const {
    return if_inst;
}

If::If( const Vec<Value> &inp, RcPtr<IfInp> inp_ok, IfOut *out_ok, RcPtr<IfInp> inp_ko, IfOut *out_ko ) : inp_ok( inp_ok ), out_ok( out_ok ), inp_ko( inp_ko ), out_ko( out_ko ) {
    inp_ok->if_inst = this;
    inp_ko->if_inst = this;
    out_ok->if_inst = this;
    out_ko->if_inst = this;
    out_ok->if_inp = inp_ok.ptr();
    out_ko->if_inp = inp_ko.ptr();

    for( const Value &ch : inp )
        add_child( ch );
}

If::If( AttrClone, const If *a ) {
}

void If::get_mod_ressources( const std::function<void( Ressource *, bool)> &cb ) const {
    TODO;
}

bool If::expects_a_reg_at( int ninp ) const {
    return true;
}

void If::attr_clone( const If *a ) {
    inp_ok = static_cast<IfInp *>( a->inp_ok->clone() );
    inp_ko = static_cast<IfInp *>( a->inp_ko->clone() );
    out_ok = static_cast<IfOut *>( a->out_ok->clone() );
    out_ko = static_cast<IfOut *>( a->out_ko->clone() );

    inp_ok->if_inst = this;
    inp_ko->if_inst = this;
    out_ok->if_inst = this;
    out_ko->if_inst = this;
    out_ok->if_inp = inp_ok.ptr();
    out_ko->if_inp = inp_ko.ptr();
}

bool If::simplify_for_cg( Codegen &cg ) {
    bool res = false;

    // outputs
    for( size_t nout = out_ok->children.size(); nout--; ) {
        // look for ok and ko parts that does not change anything
        if ( out_ok->children[ nout ].inst == inp_ok && out_ko->children[ nout ].inst == inp_ko && out_ok->children[ nout ].nout == out_ko->children[ nout ].nout ) {
            Vec<Parent> pl = parents;
            for( const Parent &p : pl )
                p.inst->mod_child( p.ninp, children[ out_ok->children[ nout ].nout ] );
            res = true;
        }

        // if an if output is unused, we can remove the computation
        if ( nb_parents_on_nout( nout ) == 0 ) {
            out_ok->rem_child( nout );
            out_ko->rem_child( nout );
            rem_out( nout );
            res = true;
            continue;
        }
    }

    // inputs
    for( size_t ninp = children.size(); --ninp; ) {
        if ( inp_ok->nb_parents_on_nout( ninp ) == 0 && inp_ko->nb_parents_on_nout( ninp ) == 0 ) {
            inp_ok->rem_out( ninp );
            inp_ko->rem_out( ninp );
            rem_child( ninp );
            res = true;
        }
    }

    return res;
}

void If::write_dot( std::ostream &os ) const {
    os << "If";
}

int If::inp_corr( int nout ) const {
    Value &ch = out_ok->children[ nout ];
    Inst *inst = ch.inst.ptr();
    nout = ch.nout;

    // find inp_ok
    while ( true ) {
        if ( inst == inp_ok.ptr() )
            return nout;
        int ind = inst->inp_corr( nout );
        if ( ind < 0 )
            return -1;
        nout = inst->children[ ind ].nout;
        inst = inst->children[ ind ].inst.ptr();
    }
}

bool If::write_graph_rec( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void (std::ostream &, const Inst *)> &f, bool disp_parents ) const {
    if ( Inst::write_graph_rec( ss, seen_insts, f, disp_parents ) ) {
        ss << "  subgraph cluster_" << this << "_OK {";
        out_ok->write_graph_rec( ss, seen_insts, f, disp_parents );
        ss << "  }";
        ss << "  node_" << this << " -> node_" << out_ok.ptr() << " [color=blue,style=dotted,label=\"OK\"];\n";

        ss << "  subgraph cluster_" << this << "_KO {";
        out_ko->write_graph_rec( ss, seen_insts, f, disp_parents );
        ss << "  }";
        ss << "  node_" << this << " -> node_" << out_ko.ptr() << " [color=blue,style=dotted,label=\"KO\"];\n";
        return true;
    }
    return false;
}

bool If::can_be_inlined() const {
    return false;
}

void If::write_code( StreamSep &ss, Codegen &cg ) {
    //    // some registers may have to be pre-declared
    //    for( int nout = 0; nout < nb_outputs(); ++nout ) {
    //        int ninp = inp_corr( nout );
    //        if ( ninp < 0 ) {
    //            // get a new reg for the ok part
    //            Reg *reg = 0;
    //            Type *type = 0;
    //            Inst::follow_thread( out_ok->children[ nout ], [&]( Inst *inst, int nout, bool last ) {
    //                if ( last ) {
    //                    type = inst->type( nout );
    //                    reg = cg.reg( inst, nout );
    //                }
    //            } );
    //            // copy reg for the ko part
    //            Inst::follow_thread( out_ko->children[ nout ], [&]( Inst *inst, int nout, bool last ) {
    //                if ( last )
    //                    inst->cd.out_regs.secure_set( nout, reg );
    //            } );
    //            // declare
    //            ss << cg.repr( type ) << " " << cg.repr( reg ) << ";";
    //            reg->pre_declared = true;
    //        }
    //    }

    //    //    // variables of in which usable references are base must be pre-declared
    //    //    auto fd = [&]( Ptr_on_ref *pr ) { cg.decl_reg( &ss, pr->children[ 0 ] ); };
    //    //    for( const Value &ch : out_ok->children )
    //    //        ch.inst->find_usable_ref_rec( ch.nout, fd, 0 );
    //    //    for( const Value &ch : out_ko->children )
    //    //        ch.inst->find_usable_ref_rec( ch.nout, fd, 0 );

    // code for ok and for ko
    std::ostringstream os_ok;
    std::ostringstream os_ko;
    StreamSep ss_ok( os_ok, ss.beg + "    " );
    StreamSep ss_ko( os_ko, ss.beg + "    " );
    std::unique_ptr<Codegen> cg_ok( cg.new_child() );
    std::unique_ptr<Codegen> cg_ko( cg.new_child() );
    cg_ok->write_block( ss_ok, out_ok.ptr() );
    cg_ko->write_block( ss_ko, out_ko.ptr() );
    std::string s_ok = os_ok.str();
    std::string s_ko = os_ko.str();

    // write
    if ( s_ko.empty() ) {
        ss << "if ( " << cg.repr( children[ 0 ], PRIO_paren ) << " ) {";
        *ss.stream << s_ok;
        ss << "}";
    } else if ( s_ok.empty() ) {
        ss << "if ( ! " << cg.repr( children[ 0 ], PRIO_Logical_not ) << " ) {";
        *ss.stream << s_ko;
        ss << "}";
    } else {
        ss << "if ( " << cg.repr( children[ 0 ], PRIO_paren ) << " ) {";
        *ss.stream << s_ok;
        ss << "} else {";
        *ss.stream << s_ko;
        ss << "}";
    }

    // store regs
    for( int nout = 0; nout < nb_outputs(); ++nout )
        cd.out_regs.secure_set( nout, out_ok->children[ nout ].inst->cd.out_regs.secure_get( out_ok->children[ nout ].nout, 0 ) );
}

void If::get_out_insts( Deque<Inst *> &outs ) {
    outs.push_back_unique( out_ok.ptr() );
    outs.push_back_unique( out_ko.ptr() );
}

//bool If::get_cpt_ref( int nout, Ref_count &rc ) const {
//    Ref_count rc_ok;
//    if ( out_ok->children[ nout ].get_cpt_ref( rc_ok ) == false )
//        return false;

//    Ref_count rc_ko;
//    if ( out_ko->children[ nout ].get_cpt_ref( rc_ko ) == false )
//        return false;

//    rc.add_conditionnal( children[ 0 ], true , rc_ok );
//    rc.add_conditionnal( children[ 0 ], false, rc_ko );
//    return nout < int( children.size() ) ? children[ nout ].get_cpt_ref( rc ) : true;
//}

//Type *If::type( int nout ) const {
//    return out_ok->children[ nout ].type();
//}

//Value If::size( int nout ) const {
//    return out_ok->children[ nout ].size();
//}

//Value If::alig( int nout ) const {
//    return out_ok->children[ nout ].alig();
//}


Value new_If( const Vec<Value> &inp, IfInp *inp_ok, IfOut *out_ok, IfInp *inp_ko, IfOut *out_ko, int nout ) {
    //    // if cond is a not(...) and is not used inside
    //    if ( dynamic_cast<UnaOp<NotLog> *>( inp[ 0 ].inst.ptr() ) &&  )
    //        return new If( Vec<Val>( inp[ 0 ].inst->children[ 0 ] ).concat( inp.from( 1 ) ), inp_ko, out_ko, inp_ok, out_ok );
    return { new If( inp, inp_ok, out_ok, inp_ko, out_ko ), nout, inp[ nout ].type };
}

int If::nb_outputs() const {
    return out_ok->children.size();
}
