#pragma once

#include "Value.h"


/**
*/
class Ressource {
public:
    Ressource( const char *desc );

    void        write_to_stream( std::ostream &os ) const;

    Value       last_writers;
    Vec<Value>  last_readers;
    const char *desc;
};

