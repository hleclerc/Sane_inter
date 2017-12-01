#pragma once

#include "Variable.h"

/**
*/
struct ListOfTypesOrSurdefLists {
    void write_to_stream( std::ostream &os ) const { os << lst; }

    Vec<Variable> lst;
};
