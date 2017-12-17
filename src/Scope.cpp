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

size_t Scope::nb_scopes_to_catch() const {
    size_t nb_l = 1;
    for( const Scope *s = this; s && s->type != ScopeType::TRY; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_break() const {
    const Scope *loop = this;
    while ( loop->type != ScopeType::WHILE && loop->type != ScopeType::FOR_BEG ) {
        loop = loop->parent_interp( false );
        if ( ! loop )
            return gvm->add_error( "break must be called inside a loop (while, for, ...)" ), 0;
    }

    size_t nb_l = 1;
    for( const Scope *s = this; s != loop; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_cont() const {
    const Scope *loop = this;
    while ( loop->type != ScopeType::WHILE && loop->type != ScopeType::FOR_EXE ) {
        loop = loop->parent_interp( false );
        if ( ! loop )
            return gvm->add_error( "continue must be called inside a loop (while, for, ...)" ), 0;
    }

    size_t nb_l = 1;
    for( const Scope *s = this; s != loop; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_ret() const {
    const Scope *caller = this;
    while ( caller->type != ScopeType::CALL )
        caller = caller->parent_interp( false );

    size_t nb_l = 1;
    for( const Scope *s = this; s != caller; s = s->parent )
        ++nb_l;
    return nb_l;
}

Scope *Scope::parent_interp( bool squeeze_for_beg ) const {
    if ( type == ScopeType::FOR_EXE ) {
        int nb_for = 1;
        for( Scope *s = parent; s; s = s->parent ) {
            if ( s->type == ScopeType::FOR_BEG ) {
                if ( --nb_for == 0 )
                    return squeeze_for_beg ? s->parent : s;
            } else if ( s->type == ScopeType::FOR_EXE )
                ++nb_for;
        }
    }
    if ( type == ScopeType::IF_EXE )
        return parent->parent;
    return parent;
}
