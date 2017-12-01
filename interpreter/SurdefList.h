#pragma once

#include "Variable.h"

/**
*/
class SurdefList {
public:
    SurdefList();

    bool           only_contains_defs_named( Vm *vm, const Rc_string &name ) const;
    void           write_to_stream         ( std::ostream &os ) const { os << "SurdefList(self=" << self << ")"; }
    Rc_string      possible_names          ( Vm *vm ) const;
    bool           has_wildcards           ( Vm *vm ) const;
    bool           eq                      ( Scope *scope, SurdefList *sl );
    bool           eq                      ( Scope *scope, Type *type );

    Vec<Variable>  lst;   // list of definitions (class, def, ...). sorted by (known) decreasing pertinence
    Variable       self;  // self
    Vec<Variable>  args;  //  as in Foo[ arg1, arg2, ... ]
    Vec<Rc_string> names;
};
