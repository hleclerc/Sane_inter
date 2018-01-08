#pragma once

#include "../System/Stream.h"
class Type;

/**
*/
class Reg {
public:
    Reg( Type *type, int num );

    virtual void write_to_stream( std::ostream &os ) const;

    Type        *type;
    int          num;
};
