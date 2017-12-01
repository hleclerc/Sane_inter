#include "Type_ListOfTypesOrSurdefLists.h"
#include "System/rcast.h"
#include "Vm.h"

Type_ListOfTypesOrSurdefLists::Type_ListOfTypesOrSurdefLists() : Type_BaseBin( "ListOfTypesOrSurdefLists" ) {
}

Rc_string Type_ListOfTypesOrSurdefLists::checks_type_constraint(Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const {
    ListOfTypesOrSurdefLists *l = rcast( self.ptr() );
    for( const Variable &surdef : l->lst )
        if ( surdef.valid_constraint_for( scope, tested_var, tci ).empty() )
            return {};
    return "has no type correspondance";
}

