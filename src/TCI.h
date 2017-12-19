#pragma once

#include <unordered_map>
#include "Variable.h"

/***/
struct TCI {
    TCI() : nb_conversions( 0 ) {}

    std::unordered_map<RcString,Variable> proposals;
    unsigned                              nb_conversions;
};
