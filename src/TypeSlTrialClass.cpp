#include "TypeSlTrialClass.h"
#include "SlTrialClass.h"
#include "Class.h"

TypeSlTrialClass::TypeSlTrialClass() : Type( "SlTrialClass" ) {
}

bool TypeSlTrialClass::get_condition( const Variable &self ) const {
    return true;
    // return reinterpret_cast<SlTrialClass *>( self.ptr() )->condition;
}

void TypeSlTrialClass::get_fail_info( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const {
    TODO;
    //    SlTrialClass *tr = rcast( self.ptr() );
    //    Class *def = rcast( tr->def.ptr() );
    //    offset = def->offset;
    //    source = def->source;
    //    msg = tr->msg;
}

unsigned TypeSlTrialClass::get_nb_conversions( const Variable &self ) const {
    TODO;
    return 0;
//    SlTrialClass *tr = rcast( self.ptr() );
//    return tr->tci.nb_conversions;
}
