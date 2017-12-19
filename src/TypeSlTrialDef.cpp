#include "TypeSlTrialDef.h"
#include "SlTrialDef.h"
#include "Def.h"

TypeSlTrialDef::TypeSlTrialDef() : Type( "SlTrialDef" ) {
}

bool TypeSlTrialDef::get_condition( const Variable &self ) const {
    return true;
    // return reinterpret_cast<SlTrialDef *>( self.ptr() )->condition;
}

void TypeSlTrialDef::get_fail_info( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const {
    TODO;
    //    SlTrialDef *tr = rcast( self.ptr() );
    //    Def *def = rcast( tr->def.ptr() );
    //    offset = def->offset;
    //    source = def->source;
    //    msg = tr->msg;
}

unsigned TypeSlTrialDef::get_nb_conversions( const Variable &self ) const {
    TODO;
    return 0;
    //    SlTrialDef *tr = rcast( self.ptr() );
    //    return tr->tci.nb_conversions;
}
