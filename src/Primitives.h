#pragma once

#include <functional>
class Variable;
class Scope;
class Type;

struct Primitive_decl {
    Primitive_decl( const char *name, std::function<Type*()> func );

    Primitive_decl        *prev;
    const char            *name;
    std::function<Type*()> func;
};

struct Primitive_func {
    Primitive_func( const char *name, std::function<Variable()> func );

    Primitive_func           *prev;
    const char               *name;
    std::function<Variable()> func;
};

extern Primitive_decl *last_Primitive_decl;
extern Primitive_func *last_Primitive_func;
