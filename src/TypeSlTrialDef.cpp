#include "TypeSlTrialDef.h"
#include "SlTrialDef.h"
#include "Def.h"

TypeSlTrialDef::TypeSlTrialDef() : Type( "SlTrialDef" ) {
}

Type::CondVal TypeSlTrialDef::get_condition( const Variable &self ) const {
    return self.rcast<SlTrialDef>()->condition;
}

void TypeSlTrialDef::get_fail_info( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const {
    SlTrialDef *tr = self.rcast<SlTrialDef>();
    Def *def = tr->def.rcast<Def>();
    offset = def->offset;
    source = def->source;
    msg = tr->msg;
}

unsigned TypeSlTrialDef::get_nb_conversions( const Variable &self ) const {
    TODO;
    return 0;
    //    SlTrialDef *tr = rcast( self.ptr() );
    //    return tr->tci.nb_conversions;
}
