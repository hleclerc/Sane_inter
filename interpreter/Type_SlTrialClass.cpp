#include "Type_SlTrialClass.h"
#include "System/rcast.h"
#include "Class.h"

Type_SlTrialClass::Type_SlTrialClass() : Type_BaseBin( "SlTrialClass" ) {
}

bool Type_SlTrialClass::get_condition( Scope *scope, const Variable &self ) const {
    return reinterpret_cast<SlTrialClass *>( self.ptr() )->condition;
}

void Type_SlTrialClass::get_fail_info( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const {
    SlTrialClass *tr = rcast( self.ptr() );
    Class *def = rcast( tr->def.ptr() );
    offset = def->offset;
    source = def->source;
    msg = tr->msg;
}

unsigned Type_SlTrialClass::get_nb_conversions( Scope *scope, const Variable &self ) const {
    SlTrialClass *tr = rcast( self.ptr() );
    return tr->tci.nb_conversions;
}
