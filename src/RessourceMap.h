#pragma once

#include "Ressource.h"
#include <map>

/**
*/
class RessourceMap {
public:
    using MVR = std::map<Value,Ressource>;

    RessourceMap();
    ~RessourceMap();

    void        get_prs_on_file_content( const Value &fd, const std::function<void( Ressource * )> &cb );
    void        get_prs_on_file_cursor ( const Value &fd, const std::function<void( Ressource * )> &cb );

    void        visit                 ( std::function<void( Ressource *rs )> visitor );

private:
    MVR         file_cursors; ///< fd => ressource (representation of the file cursor)
    Ressource  *file_content;
};


