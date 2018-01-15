#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "../gvm.h"
#include "ReadFd.h"
#include "Cst.h"

ReadFd::ReadFd( const Value &fd, const Value &ptr, const Value &len ) {
    add_child( fd  );
    add_child( ptr );
    add_child( len );
}

ReadFd::ReadFd( AttrClone, const ReadFd *orig ) {
}

void ReadFd::get_mod_ressources( const std::function<void(Ressource *, bool)> &cb ) const {
    // we read the content of fd
    gvm->ressource_map.get_prs_on_file_content( children[ 0 ], [&]( Ressource *rs ) {
        cb( rs, false );
    } );
    // we write the cursor of fd
    gvm->ressource_map.get_prs_on_file_cursor( children[ 0 ], [&]( Ressource *rs ) {
        cb( rs, true );
    } );
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
    if ( nout )
        return nout + 2;
    return 1;
}

//bool ReadFd::expects_a_reg_at( int ninp ) const {
//    return ninp == 0;
//}

//bool ReadFd::can_be_inlined() const {
//    return false;
//}

int ReadFd::nb_outputs() const {
    return children.size() - 2;
}
