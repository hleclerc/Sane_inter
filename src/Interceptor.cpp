#include "System/RaiiSave.h"
#include "Interceptor.h"
#include "gvm.h"

void Interceptor::run( const std::function<void ()> &func ) {
    // new global interception context
    auto _r0 = raii_save( gvm->inter_date, gvm->inter_date + 1 );
    auto _r1 = raii_save( gvm->interceptor, this );

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
    for( auto p = mod_ressources.begin(); p != mod_ressources.end(); ) {
        if ( p->first->state == p->second.o ) {
            p = mod_ressources.erase( p );
        } else {
            p->second.n = p->first->state;
            p->first->state = p->second.o;
            ++p;
        }
    }

    // new_breaks = breaks;
}
