#include "../System/DotOut.h"
#include "../Value.h"
#include <fstream>

Inst::~Inst() {
}

void Inst::add_child( const Value &ch ) {
    children << ch;
}

void Inst::add_dep( const RcPtr<Inst> &inst ) {
    deps << inst;
}

int Inst::inp_corr( int nout ) const {
    return -1;
}

Inst::Inp Inst::val_corr( int nout ) const {
    return {};
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

void Inst::get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    write_dot( std::cerr );
    TODO;
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

