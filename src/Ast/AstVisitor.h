#pragma once

#include "Helpers/(GenAstVisitor.ts AstDecl.txt).h"

enum {
    AST_NUMBER_FLAG_l         =  1,
    AST_NUMBER_FLAG_p         =  2
};

enum {
    CALLABLE_FLAG_self_as_arg     =  1,
    CALLABLE_FLAG_abstract        =  2,
    CALLABLE_FLAG_static          =  4,
    CALLABLE_FLAG_export          =  8,
    CALLABLE_FLAG_global          = 16,
    CALLABLE_FLAG_virtual         = 32,
    CALLABLE_FLAG_override        = 64,
    // less common flags
    CALLABLE_FLAG_private         = 128,
    CALLABLE_FLAG_protected       = 256,
    CALLABLE_FLAG_inline          = 512,
    CALLABLE_FLAG_named_converter = 1024,
};


enum {
    ASSIGN_FLAG_static    =  1,
    ASSIGN_FLAG_const     =  2,
    ASSIGN_FLAG_type      =  4,
    ASSIGN_FLAG_export    =  8,
    ASSIGN_FLAG_global    = 16,
    ASSIGN_FLAG_ref       = 32,
    ASSIGN_FLAG_private   = 64,
    ASSIGN_FLAG_protected = 128,
};
