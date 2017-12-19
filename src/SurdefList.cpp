#include "SurdefList.h"
#include "Class.h"
#include "Def.h"
#include "gvm.h"

bool SurdefList::only_contains_defs_named( const RcString &name ) const {
    if ( lst.size() == 0 )
        return false;
    for( const Variable &var : lst ) {
        if ( var.type != gvm->type_Def )
            return false;
        Def *def = var.rcast<Def>();
        if ( def->name != name )
            return false;
    }
    return true;
}

RcString SurdefList::possible_names() const {
    std::set<RcString> set;
    for( const Variable &var : lst ) {
        if ( var.type == gvm->type_Def ) {
            Def *def = var.rcast<Def>();
            set.insert( def->name );
        } else if ( var.type == gvm->type_Class ) {
            Class *def = var.rcast<Class>();
            set.insert( def->name );
        }
    }

    RcString res;
    for( const RcString &n : set )
        res = res + ( res.empty() ? "" : "|" ) + n;
    return res;
}
