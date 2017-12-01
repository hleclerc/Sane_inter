#include "Type_Wildcard.h"
#include "System/rcast.h"
#include "Scope.h"
#include "TCI.h"
#include "Vm.h"

Type_Wildcard::Type_Wildcard() : Type_BaseBin( "Wildcard" ) {
}

Rc_string Type_Wildcard::checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const {
    Wildcard *wc = rcast( self.ptr() );
    tci.proposals[ wc->name ] = scope->vm->new_Type( tested_var.type );
    return {};
}

