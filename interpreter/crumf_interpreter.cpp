#include "System/File_reader.h"
#include "System/LVec.h"
#include <fstream>
#include "Vm.h"

#ifndef INSTALL_DIR
#define INSTALL_DIR std::experimental::filesystem::path( __FILE__ ).parent_path().parent_path()
#endif

int main( int argc, char **argv ) {
    bool want_graph_only = 0;
    Vm vm;

    //
    vm.includes << ( INSTALL_DIR / "lib" ).string();
    vm.argc = argc - 1;
    vm.argv = argv + 1;

    // base variables
    if ( want_graph_only == false ) {
        std::ifstream imports( INSTALL_DIR / "lib" / "globals" / "imports" );
        std::string line;
        while ( std::getline( imports, line ) )
            if ( line.empty() == false && line[ 0 ] != '#' )
                vm.import( ( INSTALL_DIR / "lib" / "globals" / line ).string() );
        vm.init_mode = false;
    }

    // user script(s)
    try {
        vm.import( argv[ 1 ], want_graph_only );
    } catch ( const Scope::Exception &e ) {
        if ( ! e.silent )
            std::cerr << e.error;
    }
}
