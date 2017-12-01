#pragma once

#include "Type.h"

/**
*/
class Type_Void : public Type {
public:
    Type_Void();

    virtual void destroy( Scope *scope, const Variable &self, bool use_virtual ) override;
};

