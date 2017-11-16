#pragma once

#include "Ref.h"

/**
*/
class Variable {
public:
    Variable( const RcPtr<Ref> &ref, Type *type, SI32 offset = 0 );
    Variable( const Value &value ); // make a RefLeaf from a value
    Variable() {} // void Variable

    Variable   &operator=( const Variable &value );

    operator    bool() const { return ref; }

    RcPtr<Ref> ref;
    Type      *type;
    SI32       offset;
};

