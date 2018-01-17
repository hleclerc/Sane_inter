#pragma once

#include "RefLeaf.h"

/**
*/
class Rss {
public:
    Rss( const char *desc );

    void           write_to_stream( std::ostream &os ) const;

    RcPtr<RefLeaf> state;
    const char    *desc;
};

