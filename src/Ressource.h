#pragma once

#include "RessourceState.h"


/**
*/
class Ressource {
public:
    Ressource( const char *desc );

    void        write_to_stream( std::ostream &os ) const;

    RessourceState state;
    const char    *desc;
};

