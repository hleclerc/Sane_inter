#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "../gvm.h"
#include "WriteFd.h"
#include "Cst.h"

WriteFd::WriteFd( RessourceMap *ressource_map, const Vec<Value> &args ) {
    for( const Value &arg : args )
        add_child( arg );

    int nout = 0;
    auto af = [&]( Ressource *rs ) {
        add_child( rs->state->get() );
        rs->state->set( { this, nout++, gvm->type_Ressource, 0 } );
    };

    nb_base_args = children.size();
    ressource_map->get_prs_on_file_content( args[ 0 ], af );
    ressource_map->get_prs_on_file_cursor ( args[ 0 ], af );
}

WriteFd::WriteFd( AttrClone, const WriteFd *orig ) : nb_base_args( orig->nb_base_args ) {
}

void WriteFd::write_code( StreamSep &ss, Codegen &cg ) {
    ss.write_beg();
    if ( nb_base_args == 2 )
        *ss << cg.write_func_write_fd( children[ 1 ].type ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << ")";
    else
        *ss << cg.write_func_write_fd( 0 ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << "," << cg.repr( children[ 2 ] ) << ")";
    ss.write_end( ";" );
}

void WriteFd::write_dot( std::ostream &os ) const {
    os << "WriteFd";
}

int WriteFd::inp_corr( int nout ) const {
    return nb_base_args + nout;
}

bool WriteFd::expects_a_reg_at( int ninp ) const {
    return ninp == 0;
}

bool WriteFd::can_be_inlined() const {
    return false;
}

int WriteFd::nb_outputs() const {
    return children.size() - nb_base_args;
}

void make_WriteFd( RessourceMap *ressource_map, const Vec<Value> &args ) {
    new WriteFd( ressource_map, args );
}
