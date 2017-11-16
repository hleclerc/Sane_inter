#pragma once

#include "Ref.h"

/**
*/
class RefLeaf : public Ref {
public:
    RefLeaf( const Value &value );

    Value value;
};

