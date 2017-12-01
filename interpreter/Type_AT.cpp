#include "System/rcast.h"
#include "Type_AT.h"

Type_AT::Type_AT() : Type_BaseBin( "AT" ) {
}

bool Type_AT::is_false( Scope *scope, const Variable &self ) {
    AT *at = rcast( self.ptr() );
    return ! at->ptr->content;
}

bool Type_AT::is_true( Scope *scope, const Variable &self ) {
    AT *at = rcast( self.ptr() );
    return at->ptr->content;
}

