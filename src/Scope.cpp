#include "System/RaiiSave.h"
#include "Scope.h"
#include "gvm.h"

Scope::Scope( Scope::ScopeType type ) : parent( gvm->scope ), type( type ) {
    import = 0;
    root   = parent ? parent->root : 0;
}

Scope::~Scope() {
//    for( const auto &d : delayed_vars )
//        for( DelayedVarInScope *dv : d.second )
//            dv->scope = 0;

//    clear();

//    if ( parent ) {
//        parent->ret = ret;

//        if ( nb_breaks )
//            parent->nb_breaks = nb_breaks - 1;
//        else if ( nb_conts )
//            parent->nb_conts = nb_conts - 1;
    //    }
}

