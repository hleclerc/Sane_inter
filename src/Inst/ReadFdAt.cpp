#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "../Vm.h"
#include "ReadFdAt.h"
#include "Cst.h"

ReadFdAt::ReadFdAt( RessourceMap *ressource_map, const Value &fd, const Value &val, const Value &off_in_file, const Value &len ) {
    add_child( fd          );
    add_child( val         );
    add_child( off_in_file );
    add_child( len         );

    ressource_map->get_prs_on_file_content( fd, [&]( Ressource *rs ) { add_child( rs->state->get() ); } );
}

ReadFdAt::ReadFdAt( AttrClone, const ReadFdAt *orig ) {
}

int ReadFdAt::nb_outputs() const {
    return 1;
}

int ReadFdAt::inp_corr( int nout ) const {
    return 1;
}

//void ReadFdAt::write_code( StreamSep &ss, Codegen &cg ) {
//    ss.write_beg();
//    if ( nb_inp == 1 )
//        *ss << cg.write_func_write_fd( children[ 1 ].type ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << ")";
//    else
//        *ss << cg.write_func_write_fd( 0 ) << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << "," << cg.repr( children[ 2 ] ) << ")";
//    ss.write_end( ";" );
//}

void ReadFdAt::write_dot( std::ostream &os ) const {
    os << "ReadFdAt";
}

//bool ReadFdAt::expects_a_reg_at( int ninp ) const {
//    return ninp == 0;
//}

//bool ReadFdAt::can_be_inlined() const {
//    return false;
//}


Value make_ReadFdAt( RessourceMap *ressource_map, const Value &fd, const Value &val, const Value &off_in_file, const Value &len ) {
    return { new ReadFdAt( ressource_map, fd, val, off_in_file, len ), 0, val.type };
}
