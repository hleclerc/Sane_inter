#pragma once

#include "../Value.h"

/**
*/
class Cst : public Inst {
public:
    template<class T>
    static Value make() {
        return {};
    }

};

