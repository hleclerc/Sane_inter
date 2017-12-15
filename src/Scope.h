#pragma once

#include "System/RcString.h"
#include "Variable.h"
class AstCrepr;
class Import;

/**
*/
class Scope {
public:
    enum class ScopeType : int { ROOT, CALL, BLOCK, WHILE, TYPE_CTOR, FOR_BEG, FOR_EXE, WPC, TRY, CATCH, IF_EXE };

    Scope( ScopeType type );
    ~Scope();

    Scope     *parent;
    Import    *import;
    Scope     *root;
    ScopeType type;
};

