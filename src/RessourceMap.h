#pragma once

#include "Rss.h"
#include <map>

/**
*/
class RessourceMap {
public:
    using MVR = std::map<Value,Rss>;

    RessourceMap();
    ~RessourceMap();

    void        get_prs_on_file_content( const Value &fd, const std::function<void( Rss * )> &cb );
    void        get_prs_on_file_cursor ( const Value &fd, const std::function<void( Rss * )> &cb );

    void        visit                 ( std::function<void( Rss *rs )> visitor );

private:
    MVR         file_cursors; ///< fd => ressource (representation of the file cursor)
    Rss  *file_content;
};


