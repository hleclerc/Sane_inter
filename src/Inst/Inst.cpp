#include "../Codegen/Codegen.h"
#include "../System/DotOut.h"
#include "../System/Deque.h"
#include "../Value.h"
#include <fstream>

size_t Inst::cur_op_id = 0;
size_t Inst::date      = 0;

Inst::Inst() : op_id( 0 ) {
    creation_date = date++;
}

Inst::~Inst() {
    for( int ninp = 0; ninp < (int)children.size(); ++ninp )
        children[ ninp ].inst->parents.remove_first( Parent{ this, ninp } );
    for( int ndep = 0; ndep < (int)deps.size(); ++ndep )
        deps[ ndep ]->parents.remove_first( Parent{ this, - 1 - ndep } );
}

void Inst::add_child( const Value &ch ) {
    ch.inst->parents << Parent{ this, int( children.size() ) };
    children << ch;
}

void Inst::mod_child( int ninp, const Value &ch ) {
    children[ ninp ].inst->parents.remove_first( Parent{ this, ninp } );
    ch.inst->parents << Parent{ this, ninp };
    children[ ninp ] = ch;
}

void Inst::rem_child( int ninp ) {
    // remove link of parent of child ninp
    children[ ninp ].inst->parents.remove_first( Parent{ this, ninp } );

    // offset ninp in parents of children with index > ninp
    for( int i = children.size(); --i > ninp; )
        --children[ i ].inst->parents.find( Parent{ this, i } )->ninp;

    // remove from children list
    children.remove( ninp );
}

void Inst::rem_out( int nout, bool check_if_unused ) {
    for( Parent &p : parents ) {
        if ( check_if_unused )
            ASSERT( p.inst->children[ p.ninp ].nout != nout, "..." );
        if ( p.inst->children[ p.ninp ].nout < 0 )
            TODO;
        if ( p.inst->children[ p.ninp ].nout > nout )
            --p.inst->children[ p.ninp ].nout;
    }
}

void Inst::add_dep( const RcPtr<Inst> &inst ) {
    inst->parents << Parent{ this, int( - 1 - deps.size() ) };
    deps << inst;
}

bool Inst::all_children_with_op_id( size_t oi ) const {
    for( const Value &ch : children )
        if ( ch.inst->op_id < oi )
            return false;
    for( const RcPtr<Inst> &dep : deps )
        if ( dep->op_id < oi )
            return false;
    return true;
}

int Inst::nb_parents_on_nout( int nout ) const {
    size_t res = 0;
    for( const Parent &p : parents )
        res += p.ninp >= 0 && p.inst->children[ p.ninp ].nout == nout;
    return res;
}

int Inst::inp_corr( int nout ) const {
    return -1;
}

Inst::Inp Inst::val_corr( int nout ) const {
    return {};
}

Type *Inst::out_type( int nout ) const {
    TODO;
    return 0;
}

Inst *Inst::clone() const {
    write_dot( std::cerr << __FUNCTION__ << " " );
    TODO;
    return 0;
}

int Inst::nb_outputs() const {
    write_dot( std::cerr << __FUNCTION__ << " " );
    TODO;
    return 0;
}

void Inst::write_to_stream( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    write_dot( os );
    if ( nout >= 0 )
        os << "[" << nout << "]";
    if ( offset >= 0 )
        os << "{" << offset << "}";
    if ( children.size() ) {
        os << '(' << children[ 0 ];
        for( size_t i = 1; i < children.size(); ++i )
            os << ',' << children[ i ];
        os << ')';
    }
}

bool Inst::write_graph_rec( std::ostream &ss, std::set<const Inst *> &seen_insts, const std::function<void (std::ostream &, const Inst *)> &f, bool disp_parents ) const {
    if ( seen_insts.count( this ) )
        return false;
    seen_insts.insert( this );

    // edges
    int cpt = 0;
    for( const Value &v : children ) {
        std::ostringstream label;
        if ( v.inst->nb_outputs() > 1 || v.nout )
            label << v.nout;        
        if ( v.offset )
            label << ( label.str().size() ? "\\n" : "" ) << "off:" << v.offset;
        // label << v.type()->name;

        if ( children.size() > 1 )
            ss << "  node_" << this << ":f" << cpt++ << " -> node_" << v.inst.ptr() << " [label=\"" << label.str() << "\"];\n";
        else
            ss << "  node_" << this << " -> node_" << v.inst.ptr() << " [label=\"" << label.str() << "\"];\n";
        v.inst->write_graph_rec( ss, seen_insts, f, disp_parents );
    }

    //    if ( disp_parents ) {
    //        for( const Parent &p : parents ) {
    //            // p.inst->write_graph_rec( ss, seen_insts, f, disp_parents );

    //            if ( p.inst->children.size() > 1 )
    //                ss << "  node_" << p.inst << ":f" << p.ninp << " -> node_" << this << " [color=\"green\"];\n";
    //            else
    //                ss << "  node_" << p.inst << " -> node_" << this << " [color=\"green\"];\n";
    //        }
    //    }

    // node
    std::ostringstream label;
    write_dot( label );
    if ( f )
        f( label, this );

    if ( children.size() > 1 ) {
        for( size_t n = 0; n < children.size(); ++n )
            label << " |<f" << n << "> ";
        dot_out( ss << "  node_" << this << " [label=\"", label.str() ) << "\",shape=record];\n";
    } else
        dot_out( ss << "  node_" << this << " [label=\"", label.str() ) << "\"];\n";

    return true;
}

void Inst::write_code( StreamSep &ss, Codegen &cg ) {
    if ( nb_outputs() != 1 )
        TODO;

    Type *type = out_type( 0 );
    ss.write_beg() << cg.repr( type ) << " " << *cg.reg( this, type, 0 ) << " = ";
    StreamPrio sp{ *ss, PRIO_Assignment };
    write_inline_code( sp, cg );
    ss.write_end( ";" );

    //    if ( nb_outs_with_content() == 1 ) {
    //        // if we're going to use only the content, not the reference, we can store the content in reg
    //        if ( ! has_out_used_as_ref() ) {
    //            Reg *reg = cg.reg( this, 0, false );
    //            std::ostringstream os;
    //            StreamPrio sp( os, PRIO_paren );
    //            write_inline_code( sp, cg, 0 );
    //            reg->is_a_ref = false;
    //            reg->name = os.str();
    //            return;
    //        }
    //        // else, we make a declaration, initialized with write_inline_code
    //        Reg *reg = cg.reg( this );
    //        *ss.stream << ss.beg;
    //        if ( ! reg->pre_declared )
    //            *ss.stream << cg.repr( type( 0 ) ) << " ";
    //        *ss.stream << cg.repr( reg, PRIO_Assignment ) << " = ";
    //        StreamPrio sp( *ss.stream, PRIO_Assignment );
    //        write_inline_code( sp, cg, 0 );
    //        *ss.stream << ";" << ss.end;
    //        return;
    //    }

    //
    //    write_dot( std::cerr << __FUNCTION__ << " " );
    //    TODO;
}

void Inst::write_inline_code( StreamPrio &ss, Codegen &cg ) {
    ss << "TODO: inline code for " << *this;
}

void Inst::get_bytes( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const {
}

void *Inst::rcast( SI32 nout, Type *type, SI32 offset ) {
    write_dot( std::cerr );
    TODO;
    return 0;
}

void Inst::display_graphviz( const Vec<Inst *> &lst, const std::function<void (std::ostream &, const Inst *)> &f, const std::string &filename, bool disp_parents, bool launch ) {
    std::set<const Inst *> seen_insts;
    std::ofstream ss( filename.c_str() );
    ss << "digraph Vm {\n";
    for( const Inst *inst : lst )
        inst->write_graph_rec( ss, seen_insts, f, disp_parents );
    ss << "}";

    ss.close();
    if ( launch )
        exec_dot( filename.c_str() );
}

void Inst::dfs( const Vec<Inst *> &lst, const std::function<void (Inst *)> &f, bool deep, bool f_after, bool need_inc_ref ) {
    ++cur_op_id;
    for( Inst *inst : lst )
        dfs_rec( inst, f, deep, f_after, need_inc_ref );
}

void Inst::dfs_rec( Inst *inst, const std::function<void (Inst *)> &f, bool deep, bool f_after, bool need_inc_ref ) {
    if ( inst->op_id == Inst::cur_op_id )
        return;
    inst->op_id = Inst::cur_op_id;

    RcPtr<Inst> tp( need_inc_ref ? inst : 0 );

    if ( ! f_after )
        f( inst );

    if ( deep ) {
        Deque<Inst *> outs;
        inst->get_out_insts( outs );
        for( Inst *out : outs )
            dfs_rec( out, f, deep, f_after, need_inc_ref );
    }

    for( const Value &v : inst->children )
        dfs_rec( v.inst.ptr(), f, deep, f_after, need_inc_ref );

    if ( f_after )
        f( inst );
}

Inst *Inst::parent_out_inst() const {
    TODO;
    return 0;
}

bool Inst::simplify_for_cg( Codegen &cg ) {
    return false;
}


void Inst::get_out_insts( Deque<Inst *> &outs ) {
}

void Inst::externalize( Inst *inst, size_t ninp ) {
    write_dot( std::cerr << __FUNCTION__ << " " );
    TODO;
}

