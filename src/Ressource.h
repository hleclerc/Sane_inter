#pragma once

#include "Value.h"


/**
*/
class Ressource {
public:
    Ressource( const char *desc );

    void        write_to_stream( std::ostream &os ) const;

    Vec<Value>  last_readers;
    Value       last_writer;
    const char *desc;
};

