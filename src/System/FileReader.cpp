//// nsmake lib_name stdc++fs
#include "FileReader.h"
#include <fstream>

FileReader::FileReader( const path &name ) {
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

FileReader::~FileReader() {
    delete [] data;
}

FileReader::operator bool() const {
    return size != size_t( -1 );
}
