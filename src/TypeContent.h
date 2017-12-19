#pragma once

#include "System/LString.h"
#include "System/LVec.h"
#include "Variable.h"
#include <map>
class Class;

/**
*/
struct TypeContent {
    struct Attribute {
        RcString        name;
        Type           *type;
        SI32            off;   ///< in bits
        Variable::Flags flags;
        Attribute       *prev;
        Attribute       *next;
    };
    using MA  = std::map<RcString,Attribute>;
    using MSA = std::map<RcString,Variable *>;

    TypeContent() { last_attribute = 0; first_attribute = 0; orig_class = 0; }

    void            write_to_stream( std::ostream &os ) const { os << name; }

    LString         name;
    MSA             methods;
    MA              attributes;
    Class          *orig_class;
    Vec<Variable *> parameters;        ///< template arguments
    Attribute      *last_attribute;
    Attribute      *first_attribute;
    MSA             static_attributes;
};
