#include "Type_DelayedImportedVar.h"
#include "System/rcast.h"
#include "Scope.h"
#include "Vm.h"

Type_DelayedImportedVar::Type_DelayedImportedVar() : Type_BaseBin( "DelayedImportedVar" ) {
}

Variable Type_DelayedImportedVar::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    DelayedImportedVar *div = rcast( self.ptr() );

    // look up for div->import->name
    for( const Import::Export &e : div->import->exports )
        if ( Rc_string( e.name ) == div->name )
            return e.var;

    // not found ? maybe it's because the import is not finished
    if ( div->import->finished == false ) {
        // ERROR("vds");
        return scope->add_error( "found no variable '{}' in recursive import '{}'. Variable is used too early ?", div->name, div->import->filename ), scope->vm->ref_error;
    }
    return scope->add_error( "there's no exported variable '{}' in file '{}'", div->name, div->import->filename ), scope->vm->ref_error;
}

