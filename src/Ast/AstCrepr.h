#pragma once

#include "../System/RcString.h"

/**
*/
struct AstCrepr {
    operator bool() const { return ! code.empty(); }

    RcString names;
    RcString code;
};
