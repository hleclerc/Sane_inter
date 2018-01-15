#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "../gvm.h"
#include "ReadFdAt.h"
#include "Cst.h"

ReadFdAt::ReadFdAt(const Value &fd, const Value &ptr, const Value &off_in_file, const Value &len ) {
    add_child( fd  );
    add_child( ptr );
    add_child( off_in_file );
    add_child( len );
}

ReadFdAt::ReadFdAt( AttrClone, const ReadFdAt *orig ) {
}

void ReadFdAt::get_mod_ressources( const std::function<void( Ressource *rs, bool write )> &cb ) const {
    // we read the content of fd
    gvm->ressource_map.get_prs_on_file_content( children[ 0 ], [&]( Ressource *rs ) {
        cb( rs, false );
    } );
}

int ReadFdAt::nb_outputs() const {
    return children.size() - 3;
}

int ReadFdAt::inp_corr( int nout ) const {
    if ( nout )
        return nout + 3;
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

