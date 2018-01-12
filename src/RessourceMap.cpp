#include "RessourceMap.h"

RessourceMap::RessourceMap() {
}

Ressource *RessourceMap::file_content_ressource_for( const Value &fd ) {
    auto iter = file_contents.find( fd );
    if ( iter == file_contents.end() )
        iter = file_contents.insert( iter, std::make_pair( fd, Ressource( "FileContent" ) ) );
    return &iter->second;
}

Ressource *RessourceMap::file_cursor_ressource_for( const Value &fd ) {
    auto iter = file_cursors.find( fd );
    if ( iter == file_cursors.end() )
        iter = file_cursors.insert( iter, std::make_pair( fd, Ressource( "FileCursor" ) ) );
    return &iter->second;
}
