#include "System/RaiiSave.h"
#include "Interceptor.h"
#include "gvm.h"

void Interceptor::run( const std::function<void ()> &func ) {
    // new global interception context
    auto _r0 = raii_save( RefLeaf::inter_date, RefLeaf::inter_date + 1 );
    auto _r1 = raii_save( RefLeaf::interceptor, this );

    // do the stuff
    func();

    // save the new values, restore the refs
    for( auto p = mod_refs.begin(); p != mod_refs.end(); ) {
        if ( p->first->value == p->second.o ) {
            p = mod_refs.erase( p );
        } else {
            p->second.n = p->first->value;
            p->first->value = p->second.o;
            ++p;
        }
    }

    // save and remove new mod fds
    //    for( auto p = mod_mod_fds.begin(); p != mod_mod_fds.end(); ++p ) {
    //        auto iter = gvm->mod_fds.find( p->first );
    //        p->second.n = iter->second;
    //        if ( p->second.o ) {
    //            TODO;
    //        } else
    //            gvm->mod_fds.erase( iter );
    //    }

    // new_breaks = breaks;
}
