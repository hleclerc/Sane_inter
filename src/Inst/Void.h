#pragma once

#include "../Value.h"

/**
*/
class Void : public Inst {
public:
    virtual void write_dot( std::ostream &os, Type *type = 0, int offset = 0 ) const override;
};

Value make_Void( Type *type );
Value make_Void();
