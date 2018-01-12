#pragma once

#include "Ressource.h"
#include <map>

/**
*/
class RessourceMap {
public:
    using MVR = std::map<Value,Ressource>;

    RessourceMap();

    void       pot_rs_on_file_content( const Value &fd, const std::function<void( Ressource * )> &cb );
    void       pot_rs_on_file_cursor ( const Value &fd, const std::function<void( Ressource * )> &cb );

    MVR        file_cursors;
    MVR        file_contents;
};


