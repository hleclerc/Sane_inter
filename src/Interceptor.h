#pragma once

#include "Variable.h"
#include "RefLeaf.h"
#include <map>

/**
*/
class Interceptor {
public:
    struct Interception {
        Value o; ///< old value
        Value n; ///< new value
    };
    struct Break {
        size_t nb_l; ///< 2 * ( nb_loops - 1 ) + is_a_break
        size_t date;
        Value  cond;
    };

    using MapInter = std::map<RcPtr<RefLeaf>,Interception>;

    void       run( const std::function<void()> &func );

    PI64       inter_date;
    MapInter   mod_refs;
    Vec<Break> breaks;
};

