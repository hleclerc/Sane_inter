#pragma once

#include "Ressource.h"
#include <map>

/**
*/
class RessourceMap {
public:
    using MVR = std::map<Value,Ressource>;

    RessourceMap();

    Ressource *file_content_ressource_for( const Value &fd );
    Ressource *file_cursor_ressource_for ( const Value &fd );

    MVR        file_cursors;
    MVR        file_contents;
};


