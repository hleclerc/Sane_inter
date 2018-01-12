#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "../Vm.h"
#include "ReadFd.h"
#include "Cst.h"

ReadFd::ReadFd( const Value &fd, const Value &ptr, const Value &len ) {
    add_child( fd  );
    add_child( ptr );
    add_child( len );
}

ReadFd::ReadFd( AttrClone, const ReadFd *orig ) {
}

void ReadFd::get_mod_ressources( const std::function<void( const Ressource *rs, bool write )> &cb ) const {
    cb( children[ 0 ], RessourceModifierType::MOD_RD_WITH_MOD_OF_CUR );
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

//int ReadFd::inp_corr( int nout ) const {
//    return 3;
//}

//bool ReadFd::expects_a_reg_at( int ninp ) const {
//    return ninp == 0;
//}

//bool ReadFd::can_be_inlined() const {
//    return false;
//}

int ReadFd::nb_outputs() const {
    return children.size() - 2;
}
