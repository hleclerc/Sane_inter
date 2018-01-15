#pragma once

#include "Value.h"


/**
*/
class RessourceState {
public:
    void        write_to_stream ( std::ostream &os ) const;
    bool        operator==      ( const RessourceState &that ) const;
    Value       make_single_inst() const;

    Vec<Value>  last_readers;
    Value       last_writer;
};

