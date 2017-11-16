//// nsmake lib_name stdc++fs
#include "File_reader.h"
#include <fstream>

File_reader::File_reader( const Rc_string &name ) : File_reader( path( std::string( name ) ) ) {
}

File_reader::File_reader( const char *name ) : File_reader( path( name ) ) {
}

File_reader::File_reader( const path &name ) {
    std::ifstream f( name.c_str() );
    if ( not f ) {
        data = new char[ 1 ];
        data[ 0 ] = 0;
        size = -1;
        return;
    }

    f.seekg( 0, std::ios::end );
    size = f.tellg();
    f.seekg( 0, std::ios::beg );

    data = new char[ size + 1 ];
    f.read( data, size );
    data[ size ] = 0;
}

File_reader::~File_reader() {
    delete [] data;
}

File_reader::operator bool() const {
    return size != size_t( -1 );
}
