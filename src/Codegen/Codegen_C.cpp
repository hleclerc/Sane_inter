#include "../Inst/Gatherer.h"
#include "../Type.h"
#include "../gvm.h"

#include "Codegen_C.h"
#include "Reg.h"

#include <cmath>
#include <queue>

struct CmpSchedInst {
    bool operator()( const Inst *a, const Inst *b ) {
        return a->creation_date > b->creation_date;
    }
};

Codegen_C::Codegen_C( Codegen_C *parent ) : parent( parent ) {
    nb_reg = parent ? parent->nb_reg : 0;

    // base arythmetic types
    #define BT( T ) type_reprs[ gvm->type_##T ] = #T;
    #include "../ArythmeticTypes.h"
    #undef BT

    add_include( "<stdint.h>" );
    add_include( "\"src/System/Stream.h\"" );

    declarations << "typedef float       FP32;\n";
    declarations << "typedef double      FP64;\n";
    declarations << "typedef long double FP80;\n";
    declarations << "typedef bool        Bool;\n";
    declarations << "typedef int8_t      SI8 ;\n";
    declarations << "typedef int16_t     SI16;\n";
    declarations << "typedef int32_t     SI32;\n";
    declarations << "typedef int64_t     SI64;\n";
    declarations << "typedef uint8_t     PI8 ;\n";
    declarations << "typedef uint16_t    PI16;\n";
    declarations << "typedef uint32_t    PI32;\n";
    declarations << "typedef uint64_t    PI64;\n";
    declarations << "typedef char        Char;\n";
}

void Codegen_C::gen_code_for( const Vec<Inst *> &targets ) {
    RcPtr<Inst> gatherer = new Gatherer;
    ++Inst::cur_op_id;
    for( Inst *inst : targets )
        gatherer->add_child( Value( inst->clone(), 0, gvm->type_Void ) );

    base_simplifications( gatherer.ptr() );

    Inst::display_graphviz( gatherer.ptr() );

    StreamSep st( main_block, "    " );
    write_block( st, gatherer->children.map( []( const Value &val ) { return val.inst.ptr(); } ) );
}

String Codegen_C::code() {
    String res;

    // inludes
    for( const String &include : includes  )
        res += "#include " + include + "\n";

    // declarations
    if ( res.size() )
        res += "\n";
    res += declarations.str();

    // code
    if ( res.size() )
        res += "\n";
    res += "int main( int argc, char **argv ) {\n";
    res += main_block.str();
    res += "}\n";
    return res;
}

void Codegen_C::write_repr( std::ostream &os, Type *type ) {
    if ( parent )
        return parent->write_repr( os, type );

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

void Codegen_C::write_repr( std::ostream &os, const Value &value, int prio, int flags ) {
    Type *reg_type = 0;
    std::function<void(StreamPrio &)> reg_writer;
    if ( Reg *reg = value.inst->cd.out_regs.secure_get( value.nout, 0 ) ) {
        reg_type = reg->type;
        reg_writer = [reg]( StreamPrio &ss ) { ss << *reg; };
    } else {
        reg_type = value.inst->out_type( value.nout );
        reg_writer = [&]( StreamPrio &ss ) { value.inst->write_inline_code( ss, *this, value.nout, flags ); };
    }

    StreamPrio ss( os, prio );
    if ( write_repr_rec( ss, reg_writer, reg_type, value.type, value.offset ) )
        return;

    P( value );
    TODO;
}

Reg *Codegen_C::new_reg_for( Inst *inst, Type *type, int nout ) {
    Reg *&res = inst->cd.out_regs.secure_get( nout, 0 );
    if ( ! res )
        res = new Reg( type, nb_reg++ );

    return res;
}

void Codegen_C::write_func_itoa() {
    if ( parent )
        return parent->write_func_itoa();
    if ( decl_funcs.insert( "itoa" ).second == false )
        return;
    declarations << "template<class T>\n";
    declarations << "char *itoa( char *buf, T content ) {\n";
    declarations << "    if ( content == 0 ) {\n";
    declarations << "        *( buf++ ) = '0';\n";
    declarations << "        return buf;\n";
    declarations << "    }\n";
    declarations << "    if ( content < 0 ) {\n";
    declarations << "        *( buf++ ) = '-';\n";
    declarations << "        content = - content;\n";
    declarations << "    }\n";
    declarations << "    char *res = buf;\n";
    declarations << "    while( content ) {\n";
    declarations << "        *( res++ ) = '0' + content % 10;\n";
    declarations << "        content /= 10;\n";
    declarations << "    }\n";
    declarations << "    for( char *fub = res, tmp; --fub > buf; ++buf ) {\n";
    declarations << "        tmp = *fub; *fub = *buf; *buf = tmp;\n";
    declarations << "    }\n";
    declarations << "    return res;\n";
    declarations << "}\n";
}

void Codegen_C::add_include( const String &include ) {
    if ( parent )
        return parent->add_include( include );
    includes.push_back_unique( include );
}

String Codegen_C::write_func_write_fd( Type *type ) {
    if ( parent )
        return parent->write_func_write_fd( type );

    if ( decl_funcs.insert( type ? va_string( "write_fd:{}", *type ) : "write_fd" ).second == false )
        return "write_fd";

    if ( type ) {
        write_func_write_fd( 0 );
        write_func_itoa();

        String type_name = to_string( repr( type ) );
        declarations << "void write_fd( int fd, const " << type_name << " &content ) {\n";
        declarations << "    char buf[ " << type->is_signed() + std::ceil( type->mantissa_len() * log( 2 ) / log( 10 ) ) << " ];\n";
        declarations << "    char *end = itoa( buf, content );\n";
        declarations << "    write_fd( fd, buf, end - buf );\n";
        declarations << "}\n";
    } else {
        add_include( "<unistd.h>" );

        declarations << "void write_fd( int fd, const char *content, size_t len ) {\n";
        declarations << "    write( fd, content, len );\n";
        declarations << "}\n";
    }

    return "write_fd";
}

Codegen *Codegen_C::new_child() {
    return new Codegen_C( this );
}

void Codegen_C::write_block( StreamSep &os, const Vec<Inst *> &out ) {
    Vec<Inst *> sched;
    get_scheduling( sched, out );

    // Inst::display_graphviz( out );
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

bool Codegen_C::write_repr_rec( StreamPrio &ss, const std::function<void(StreamPrio&)> &reg_writer, Type *reg_type, Type *tgt_type, int tgt_offset ) {
    if ( tgt_type == reg_type ) {
        reg_writer( ss );
        return true;
    }

    for( const TypeContent::Attribute *attr = reg_type->content.data.first_attribute; attr; attr = attr->next ) {
        if ( tgt_offset < attr->off )
            return false;
        if ( write_repr_rec( ss, [attr,&reg_writer](StreamPrio &ss){ ss( PRIO_Member_selection ) << reg_writer << "." << attr->name; }, attr->type, tgt_type, tgt_offset - attr->off ) )
            return true;
    }

    return false;
}
