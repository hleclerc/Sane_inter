#include "../Codegen/Codegen.h"
#include "../Scope.h"
#include "../Type.h"
#include "../gvm.h"
#include "While.h"

WhileInp::WhileInp( While *while_inst ) : while_inst( while_inst ) {
}

WhileInp::WhileInp( AttrClone, const WhileInp *a ) {
}

//bool WhileInp::get_cpt_ref( int nout, Ref_count &rc ) const {
//    return while_inst->children[ nout ].get_cpt_ref( rc );
//}

int WhileInp::nb_outputs() const {
    return while_inst->children.size();
}

void WhileInp::write_dot( std::ostream &os ) const {
    os << "WhileInp";
}

//Inst::Inp WhileInp::val_corr( int nout ) const {
//    return Inp{ while_inst, size_t( nout ) };
//}

//Inst *WhileInp::dcast( int nout ) {
//    return while_inst->children[ nout ].dcast();
//    //    if ( cond )
//    //        return while_inst->children[ nout ].dcast();
//    //    return while_inst->out_inst->children[ nout ].dcast();
//}

void WhileInp::write_code( StreamSep &ss, Codegen &cg ) {
    for( size_t ninp = 0; ninp < while_inst->children.size(); ++ninp )
        cd.out_regs.secure_set( ninp, while_inst->children[ ninp ].inst->cd.out_regs.secure_get( while_inst->children[ ninp ].nout, 0 ) );
}

//Type *WhileInp::type( int nout ) const {
//    return while_inst->type( nout );
//}

WhileOut::WhileOut( While *while_inst, WhileInp *inp_inst ) : while_inst( while_inst ), inp_inst( inp_inst ) {
}

void WhileOut::write_dot( std::ostream &os ) const {
    os << "WhileOut";
}

void WhileOut::externalize( Inst *inst, size_t ninp ) {
    //    // get index in if_inp
    //    int ind;
    //    Value &och = inst->children[ ninp ];
    //    Type *base_out_type = och.inst->out_type( och.nout );
    //    for( size_t i = 0; ; ++i ) {
    //        if ( i == if_inst->children.size() ) {
    //            ind = if_inst->children.size();
    //            if_inst->add_child( Value( och.inst, och.nout, base_out_type, 0 ) );
    //            break;
    //        }
    //        const Value &ich = if_inst->children[ i ];
    //        if ( ich.inst == och.inst && ich.nout == och.nout && ich.type == base_out_type && ich.offset == 0 ) {
    //            ind = i;
    //            break;
    //        }
    //    }

    //    // replace ninp child in inst
    //    inst->mod_child( ninp, Value( if_inp, ind, och.type, och.offset ) );
    TODO;


    //    Value &ch = inst->children[ ninp ];
    //    int ind = while_inst->children.index_first( ch );
    //    if ( ind >= 0 ) {
    //        inst->mod_child( ninp, Value( inp_inst, ind ) );
    //    } else {
    //        while_inst->add_child( ch );
    //        inst->mod_child( ninp, Value( inp_inst, while_inst->children.size() - 1 ) );
    //    }
    //    TODO; // the other WhileOut
    //    //    int ind = while_inst->children.index_first( ch );
    //    //    if ( ind >= 0 ) {
    //    //        ch.nout = ind;
    //    //    } else {
    //    //        while_inst->add_child( ch );
    //    //        ch.nout = while_inst->children.size() - 1;
    //    //    }
    //    //    ch.inst = inc_ref( inp_inst );
}

int WhileOut::nb_outputs() const {
    return 0;
}

void WhileOut::write_code( StreamSep &ss, Codegen &cg ) {
}

WhileOut::WhileOut( AttrClone, const WhileOut *a ) {
}

Inst *WhileOut::parent_out_inst() const {
    return while_inst;
}

//bool WhileOut::get_cpt_ref( int nout, Ref_count &rc ) const {
//    return children[ nout ].get_cpt_ref( rc );
//}

While::While() {
    inp_inst = new WhileInp( this );
    out_inst = new WhileOut( this, inp_inst.ptr() );
}

While::While( AttrClone, const While *a ) {
}

void While::attr_clone( const While *a ) {
    inp_inst = static_cast<WhileInp *>( a->inp_inst->clone() );
    out_inst = static_cast<WhileOut *>( a->out_inst->clone() );

    inp_inst->while_inst = this;
    out_inst->while_inst = this;
    out_inst->inp_inst = inp_inst.ptr();
}

int While::nb_outputs() const {
    return out_inst->children.size();
}

void While::write_dot( std::ostream &os ) const {
    os << "While";
}

//Inst *While::dcast( int nout ) {
//    return children[ nout ].dcast();
//}

bool While::write_graph_rec( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void (std::ostream &, const Inst *)> &f, bool disp_parents ) const {
    if ( Inst::write_graph_rec( ss, seen_insts, f, disp_parents ) ) {
        ss << "  subgraph cluster_" << this << "_co {";
        out_inst->write_graph_rec( ss, seen_insts, f, disp_parents );
        ss << "  }";
        ss << "  node_" << this << " -> node_" << out_inst.ptr() << " [color=blue];\n";
        return true;
    }
    return false;
}

void While::write_code( StreamSep &ss, Codegen &cg ) {
    // check that output registers are all declared
    //    for( CodegenData::Reg *reg : CD( this )->out_regs )
    //        cg.decl_reg( &ss, reg );

    // variables of in which usable references are base must be pre-declared
    //    auto fd = [&]( Ptr_on_ref *pr ) { cg.decl_reg( &ss, pr->children[ 0 ] ); };
    //    for( const Value &ch : out_inst->children )
    //        ch.inst->find_usable_ref_rec( ch.nout, fd, 0 );
    //    for( const Value &ch : out_lo->children )
    //        ch.inst->find_usable_ref_rec( ch.nout, fd, 0 );

    TODO;

    //    // beg loop + code for cond
    //    std::ostringstream os_co;
    //    StreamSep ss_co( os_co, ss.beg + "    " );
    //    Codegen cg_co( &cg );
    //    cg_co.write_block( ss_co, out_inst.ptr() );
    //    std::string s_co = os_co.str();
    //    if ( s_co.empty() ) {
    //        ss << "while( " << cg.repr( out_inst->children.back(), PRIO_paren ) << " ) {";
    //    } else {
    //        ss << "while( true ) {";
    //        *ss.stream << s_co;
    //        ss << "    if ( ! " << cg.repr( out_inst->children.back(), PRIO_Logical_not ) << " )";
    //        ss << "        break;";
    //    }

    //    // code for loop
    //    Codegen cg_lo( &cg );
    //    ss.inc();
    //    cg_lo.write_block( ss, out_lo.ptr() );
    //    ss.dec();

    //    // break ?
    //    if ( out_lo->children.size() == out_inst->children.size() ) {
    //        ss << "    if ( " << cg.repr( out_lo->children.back(), PRIO_paren ) << " )";
    //        ss << "        break;";
    //    }

    //    // end loop
    //    ss << "}";
}

void While::get_out_insts( Deque<Inst *> &outs ) {
    outs.push_back_unique( out_inst.ptr() );
}

//Type *While::type( int nout ) const {
//    return children[ nout ].type();
//}

//bool While::get_cpt_ref( int nout, Ref_count &rc ) const {
//    //    // get cpt_ref through all the paths
//    //    int cpt_ref_co = 0;
//    //    if ( out_inst->children[ nout ].get_cpt_ref( cpt_ref_co ) == false )
//    //        return false;

//    //    int cpt_ref_lo = 0;
//    //    if ( out_lo->children[ nout ].get_cpt_ref( cpt_ref_lo ) == false )
//    //        return false;

//    //    int cpt_ref_wh = 0;
//    //    if ( children[ nout ].get_cpt_ref( cpt_ref_wh ) == false )
//    //        return false;

//    //    // check that they are the same
//    //    if ( cpt_ref_co != cpt_ref_lo || cpt_ref_lo != cpt_ref_wh  )
//    //        return false;
//    //    cpt_ref += cpt_ref_co;
//    TODO;
//    return true;
//}

bool While::simplify_for_cg( Codegen &cg ) {
    return false;
    //    bool res = false;

    //    // start a virtual destroy sequence for unused outputs
    //    ++Inst::cur_vd_id;
    //    size_t n = children.size();
    //    for( size_t nout = n; nout--; )
    //        if ( nb_parents_on_nout( nout ) == 0 )
    //            out_lo->children[ nout ].virtual_destroy();
    //    for( size_t nout = n; nout--; )
    //        if( inp_lo->nb_parents_on_nout( nout ) == inp_lo->cur_nb_vd( nout ) )
    //            out_inst->children[ nout ].virtual_destroy();
    //    for( size_t nout = n; nout--; ) {
    //        if( inp_inst->nb_parents_on_nout( nout ) == inp_inst->cur_nb_vd( nout ) ) {
    //            out_inst->rem_child( nout );
    //            out_lo->rem_child( nout );
    //            inp_inst->rem_out( nout, false );
    //            inp_lo->rem_out( nout, false );
    //            rem_child( nout );
    //            rem_out( nout );
    //            res = true;
    //        }
    //    }

    //    return res;
}
