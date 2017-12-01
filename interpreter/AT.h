#pragma once

#include "Variable.h"

/**
*/
struct AT {
    struct V {
        V() : content( 0 ), offset_in_bytes( 0 ) {}
        Variable::Content *content;
        size_t             offset_in_bytes;
    };

    AT( const AT & ) = delete;
    AT() { ptr = new V; }
    ~AT() { delete ptr; }

    void write_to_stream( std::ostream &os ) const { os << ptr->content << "_" << ptr->offset_in_bytes; }
    void operator=      ( const AT & ) = delete;

    V   *ptr;
};
