#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "../gvm.h"
#include "ReadFd.h"
#include "Cst.h"

ReadFd::ReadFd( RessourceMap *ressource_map, const Value &fd, const Value &val, const Value &len ) {
    add_child( fd  );
    add_child( val );
    add_child( len );

    ressource_map->get_prs_on_file_content( fd, [&]( Rss *rs ) { add_child( rs->state->get() ); } );

    int nout = 1;
    beg_cursors = children.size();
    ressource_map->get_prs_on_file_cursor( fd, [&]( Rss *rs ) {
        Value old = rs->state->get();
        rs->state->set( Value( this, nout++, old.type, 0 ) );
        add_child( old );
    } );
}

ReadFd::ReadFd( AttrClone, const ReadFd *orig ) : beg_cursors( orig->beg_cursors ) {
}

//void ReadFd::write_code( StreamSep &ss, Codegen &cg ) {
//    ss.write_beg();
//    if ( nb_inp == 1 )
//        *ss << cg.write_func_write_fd( children[ 1 ].type ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << ")";
//    else
//        *ss << cg.write_func_write_fd( 0 ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << "," << cg.repr( children[ 2 ] ) << ")";
//    ss.write_end( ";" );
//}

void ReadFd::write_dot( std::ostream &os ) const {
    os << "ReadFd";
}

int ReadFd::inp_corr( int nout ) const {
    return nout ? nout + beg_cursors - 1 : 1;
}

//bool ReadFd::expects_a_reg_at( int ninp ) const {
//    return ninp == 0;
//}

//bool ReadFd::can_be_inlined() const {
//    return false;
//}

int ReadFd::nb_outputs() const {
    return 1 + ( children.size() - beg_cursors );
}

Value make_ReadFd( RessourceMap *ressource_map, const Value &fd, const Value &val, const Value &len ) {
    return { new ReadFd( ressource_map, fd, val, len ), 0, val.type };
}
