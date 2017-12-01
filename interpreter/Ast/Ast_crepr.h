#pragma once

#include "../System/Rc_string.h"

/**
*/
struct Ast_crepr {
    operator bool() const { return ! code.empty(); }

    Rc_string names;
    Rc_string code;
};
