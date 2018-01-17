#include "RessourceMap.h"

RessourceMap::RessourceMap() : file_content( 0 ) {
}

RessourceMap::~RessourceMap() {
    delete file_content;
}

void RessourceMap::get_prs_on_file_content( const Value &fd, const std::function<void (Ressource *)> &cb ) {
    if ( ! file_content )
        file_content = new Ressource( "file content" );

    cb( file_content );
}

void RessourceMap::get_prs_on_file_cursor( const Value &fd, const std::function<void (Ressource *)> &cb ) {
    // check fd is present
    auto iter = file_cursors.find( fd );
    if ( iter == file_cursors.end() )
        file_cursors.insert( iter, std::make_pair( fd, Ressource( "file cursor" ) ) );

    // call cb on potentially equal fds
    for( std::pair<const Value,Ressource> &vr : file_cursors  )
        if ( ! fd.is_not_equal( vr.first ) )
            cb( &vr.second );
}

void RessourceMap::visit( std::function<void (Ressource *)> visitor ) {
    if ( file_content )
        visitor( file_content );

    for( std::pair<const Value,Ressource> &vr : file_cursors  )
        visitor( &vr.second );
}

