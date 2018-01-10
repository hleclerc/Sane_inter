#pragma once

#include "FunctionSignature.h"
#include "System/LString.h"
#include "System/LVec.h"
#include "Variable.h"
#include <map>
#include <set>
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
    using SFS = std::set<FunctionSignature>;

    TypeContent() { last_attribute = 0; first_attribute = 0; orig_class = 0; size = 0; alig = 1; type_promote_score = -1; has_new_vtable = false; }

    void            write_to_stream( std::ostream &os ) const { os << name; }

    LString         name;
    SI32            size;
    SI32            alig;
    MSA             methods;
    MA              attributes;
    Class          *orig_class;
    Vec<Variable *> parameters;        ///< template arguments
    Attribute      *last_attribute;
    bool            has_new_vtable;
    Attribute      *first_attribute;
    SFS             abstract_methods;
    MSA             static_attributes;
    int             type_promote_score;
};
