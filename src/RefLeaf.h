#pragma once

#include "Ref.h"

/**
*/
class RefLeaf : public Ref {
public:
    RefLeaf( const Value &value );

    virtual void write_to_stream( std::ostream &os ) const;

    Value        value;
};
