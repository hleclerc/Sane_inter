#include "Type_SlTrialDef.h"
#include "System/rcast.h"
#include "Def.h"

Type_SlTrialDef::Type_SlTrialDef() : Type_BaseBin( "SlTrialDef" ) {
}

bool Type_SlTrialDef::get_condition( Scope *scope, const Variable &self ) const {
    return reinterpret_cast<SlTrialDef *>( self.ptr() )->condition;
}

void Type_SlTrialDef::get_fail_info( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const {
    SlTrialDef *tr = rcast( self.ptr() );
    Def *def = rcast( tr->def.ptr() );
    offset = def->offset;
    source = def->source;
    msg = tr->msg;
}

unsigned Type_SlTrialDef::get_nb_conversions( Scope *scope, const Variable &self ) const {
    SlTrialDef *tr = rcast( self.ptr() );
    return tr->tci.nb_conversions;
}
