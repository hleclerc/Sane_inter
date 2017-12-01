#include "System/rcast.h"
#include "SurdefList.h"
#include "CtInfo.h"
#include "Scope.h"
#include "Class.h"
#include "Vm.h"

CtInfo::CtInfo( Scope *scope, const Variable &var ) : scope( scope ), var( var ) {
}

void CtInfo::write_to_stream( std::ostream &os ) const {
    if ( var.type == scope->vm->type_SurdefList ) {
        SurdefList *sl = rcast( var.ptr() );
        os << var;
        for( const Variable &vd : sl->lst ) {
            if ( vd.type == scope->vm->type_Class ) {
                Class *def = rcast( vd.ptr() );
                for( Type *inst : def->instances ) {
                    os << " inst: [";
                    for( const Variable *p : inst->parameters )
                        os << *p;
                    os << "]";
                }
            }
        }
    } else
        os << var;
}
