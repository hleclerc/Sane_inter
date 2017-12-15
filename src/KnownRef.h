#pragma once

#include "Ref.h"

/**
*/
template<class T>
class KnownRef : public Ref {
public:

    virtual void write_to_stream( std::ostream &os ) const {
        os << data;
    }

    T data;
};
