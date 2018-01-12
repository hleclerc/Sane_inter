#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "../gvm.h"
#include "WriteFd.h"
#include "Cst.h"

WriteFd::WriteFd( const Vec<Value> &args, int nb_inp ) : nb_inp( nb_inp ) {
    for( const Value &arg : args )
        add_child( arg );
}

WriteFd::WriteFd( AttrClone, const WriteFd *orig ) : nb_inp( orig->nb_inp ) {
}

void WriteFd::get_mod_ressources( const std::function<void( Ressource *rs, bool write )> &cb ) const {
    auto fw = [&cb]( Ressource *rs ) { cb( rs, true ); };
    gvm->ressource_map.pot_rs_on_file_content( children[ 0 ], fw );
    gvm->ressource_map.pot_rs_on_file_cursor ( children[ 0 ], fw );
}

void WriteFd::write_code( StreamSep &ss, Codegen &cg ) {
    ss.write_beg();
    if ( nb_inp == 1 )
        *ss << cg.write_func_write_fd( children[ 1 ].type ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << ")";
    else
        *ss << cg.write_func_write_fd( 0 ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << "," << cg.repr( children[ 2 ] ) << ")";
    ss.write_end( ";" );
}

void WriteFd::write_dot( std::ostream &os ) const {
    os << "WriteFd";
}

int WriteFd::inp_corr( int nout ) const {
    return 3;
}

bool WriteFd::expects_a_reg_at( int ninp ) const {
    return ninp == 0;
}

bool WriteFd::can_be_inlined() const {
    return false;
}

int WriteFd::nb_outputs() const {
    return children.size() - ( 1 + nb_inp );
}
