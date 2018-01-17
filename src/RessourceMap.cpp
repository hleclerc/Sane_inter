#include "RessourceMap.h"

RessourceMap::RessourceMap() : file_content( 0 ) {
}

RessourceMap::~RessourceMap() {
    delete file_content;
}

void RessourceMap::get_prs_on_file_content( const Value &fd, const std::function<void (Rss *)> &cb ) {
    if ( ! file_content )
        file_content = new Rss( "file content" );

    cb( file_content );
}

void RessourceMap::get_prs_on_file_cursor( const Value &fd, const std::function<void (Rss *)> &cb ) {
    // check fd is present
    auto iter = file_cursors.find( fd );
    if ( iter == file_cursors.end() )
        file_cursors.insert( iter, std::make_pair( fd, Rss( "file cursor" ) ) );

    // call cb on potentially equal fds
    for( std::pair<const Value,Rss> &vr : file_cursors  )
        if ( ! fd.is_not_equal( vr.first ) )
            cb( &vr.second );
}

void RessourceMap::visit( std::function<void (Rss *)> visitor ) {
    if ( file_content )
        visitor( file_content );

    for( std::pair<const Value,Rss> &vr : file_cursors  )
        visitor( &vr.second );
}

