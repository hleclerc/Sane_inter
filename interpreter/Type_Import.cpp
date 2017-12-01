#include "DelayedImportedVar.h"
#include "System/rcast.h"
#include "Type_Import.h"
#include "GetSetter.h"
#include "Import.h"
#include "Scope.h"
#include "Vm.h"

Type_Import::Type_Import() : Type_BaseBin( "Import" ) {
}

Variable Type_Import::find_attribute( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off ) {
    Import *import = rcast( orig->ptr() );

    // look up for name
    for( const Import::Export &e : import->exports )
        if ( Rc_string( e.name ) == name )
            return e.var;

    // not found ? maybe it's because the import is not finished
    if ( import->finished == false ) {
        Variable res( scope->vm, scope->vm->type_GetSetter );
        GetSetter *gs = rcast( res.ptr() );

        gs->get = { scope->vm, scope->vm->type_DelayedImportedVar };
        DelayedImportedVar *div = rcast( gs->get.ptr() );
        div->import = import;
        div->name = name;

        return res;
    }

    return scope->add_error( 1, "there's no exported variable '{}' in file '{}'", name, import->filename ), scope->vm->ref_error;
}
