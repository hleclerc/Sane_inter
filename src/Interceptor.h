#pragma once

#include "RessourceState.h"
#include "Variable.h"
#include "RefLeaf.h"
#include <map>

/**
*/
class Interceptor {
public:
    struct RessourceChange {
        RessourceState o; ///< old value
        RessourceState n; ///< new value
    };
    struct ValueChange {
        Value o; ///< old value
        Value n; ///< new value
    };
    struct Break {
        size_t nb_l; ///< 2 * ( nb_loops - 1 ) + is_a_break
        size_t date;
        Value  cond;
    };

    using MapRRC = std::map<Ressource *,RessourceChange>;
    using MapRVC = std::map<RcPtr<RefLeaf>,ValueChange>;

    void       run( const std::function<void()> &func );

    MapRRC     mod_ressources;
    PI64       inter_date;
    MapRVC     mod_refs;
    Vec<Break> breaks;
};

