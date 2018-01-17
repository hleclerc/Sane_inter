#pragma once

#include "RefLeaf.h"

/**
*/
class Ressource {
public:
    Ressource( const char *desc );

    void        write_to_stream( std::ostream &os ) const;

    RefLeaf     state;
    const char *desc;
};

