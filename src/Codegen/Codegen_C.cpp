#include "../Inst/Inst.h"
#include "../Type.h"
#include "../gvm.h"

#include "Codegen_C.h"
#include "Reg.h"

#include <queue>

struct CmpSchedInst {
    bool operator()( const Inst *a, const Inst *b ) {
        return a->creation_date > b->creation_date;
    }
};

Codegen_C::Codegen_C() {
    nb_reg = 0;

    #define BT( T ) type_reprs[ gvm->type_##T ] = #T;
    #include "../ArythmeticTypes.h"
    #undef BT
}

void Codegen_C::gen_code_for( const Vec<Inst *> &targets ) {
    Vec<RcPtr<Inst>> clones( Rese(), targets.size() );
    ++Inst::cur_op_id;
    for( Inst *inst : targets )
        clones << inst->clone();

    StreamSep st( main_block, "    " );
    write_block( st, clones.map( []( const RcPtr<Inst> &ip ) { return ip.ptr(); } ) );
}

String Codegen_C::code() {
    String res;
    res += declarations.str();
    if ( res.size() )
        res += "\n";

    res += "int main( int argc, char **argv ) {\n";
    res += main_block.str();
    res += "}\n";
    return res;
}

void Codegen_C::write_repr( std::ostream &os, Type *type ) {
    auto iter = type_reprs.find( type );
    if ( iter == type_reprs.end() ) {
        // find a name
        String name = type->c_name();
        if ( decl_types.count( name ) ) {
            for( int cpt = 0; ; ++cpt ) {
                String trial = name + '_' + to_string( cpt );
                if ( decl_types.count( trial ) == 0 ) {
                    name = trial;
                    break;
                }
            }
        }

        // register it
        iter = type_reprs.insert( iter, std::make_pair( type, name ) );
        decl_types.insert( name );

        // struct declaration
        declarations << "struct " << name << " {\n";
        for( TypeContent::Attribute *attr = type->content.data.first_attribute; attr; attr = attr->next ) {
            // TODO: offsets != than those of C++
            declarations << "    " << repr( attr->type ) << " " << attr->name << ";\n";
        }
        declarations << "};\n";
    }

    os << iter->second;
}

void Codegen_C::write_repr( std::ostream &os, const Value &value, int prio ) {
    if ( Reg *reg = value.inst->cd.regs.secure_get( value.nout ) ) {
        write_repr_rec( os, reg, value.inst, value.type, value.offset, prio, "" );
        return;
    }
    P( value );
    TODO;
}

Reg *Codegen_C::reg( Inst *inst, Type *type, int nout ) {
    Reg *&res = inst->cd.regs.secure_get( nout, 0 );
    if ( ! res )
        res = new Reg( type, nb_reg++ );

    return res;
}

void Codegen_C::write_fd_repr( std::ostream &os, Type *type ) {
    if ( type ) {
        String type_name = to_string( repr( type ) );
        declarations << "void write_fd( int fd, const " << type_name << " &content ) {\n";
        declarations << "}\n";
    }

    os << "write_fd";
}

void Codegen_C::write_block( StreamSep &os, const Vec<Inst *> &out ) {
    Vec<Inst *> sched;
    get_scheduling( sched, out );

    Inst::display_graphviz( out );
    for( Inst *inst : sched )
        inst->write_code( os, *this );
}

void Codegen_C::get_scheduling( Vec<Inst *> &sched, const Vec<Inst *> &out ) {
    // get leaves
    std::priority_queue<Inst *,std::vector<Inst *>,CmpSchedInst> leaves;
    Inst::dfs( out, [&]( Inst *inst ) { if ( inst->children.empty() ) leaves.push( inst ); } );

    // go upside
    size_t oi = ++Inst::cur_op_id;
    while( ! leaves.empty() ) {
        Inst *inst = leaves.top();
        leaves.pop();

        if ( inst->op_id >= oi )
            continue;
        inst->op_id = oi;

        inst->cd.num_in_sched = sched.size();
        sched << inst;

        // parent insts
        for( const Inst::Parent &p : inst->parents )
            if ( p.inst->all_children_with_op_id( oi ) )
                leaves.push( p.inst );
    }
}
