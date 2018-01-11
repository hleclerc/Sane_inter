#pragma once

#include "Variable.h"
#include "RefLeaf.h"
#include <map>

/**
*/
class Interceptor {
public:
    struct ValChange {
        Value o; ///< old value
        Value n; ///< new value
    };
    struct Break {
        size_t nb_l; ///< 2 * ( nb_loops - 1 ) + is_a_break
        size_t date;
        Value  cond;
    };

    using MapRVC = std::map<RcPtr<RefLeaf>,ValChange>;
    using MapVIC = std::map<Value,ValChange>;

    void       run( const std::function<void()> &func );

    MapVIC     mod_mod_fds;
    PI64       inter_date;
    MapRVC     mod_refs;
    Vec<Break> breaks;
};

