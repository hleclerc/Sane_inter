#include "System/rcast.h"
#include "GetSetter.h"
#include "Scope.h"
#include "Vm.h"

void GetSetter::write_to_stream( std::ostream &os ) const {
    os << "get=" << get << " set=" << set << " value=" << var << " with ref " << var.content.ptr();
}

Type *GetSetter::get_type( Scope *scope, Variable *get_res ) {
    if ( ! type )  {
        if ( type_of ) {
            Variable res = type_of.apply( scope, true ).ugs( scope );
            if ( res.type != scope->vm->type_Type ) {
                res = res.apply( scope, true, {}, {}, false ).ugs( scope );
                if ( res.type != scope->vm->type_Type )
                    return scope->add_error( "typeof_... must return a type or something that gives a type" ), scope->vm->type_Error;
            }
            type = *reinterpret_cast<Type **>( res.ptr() );
        } else if ( get ) {
            var = get.apply( scope, true );
            if ( var.type->getsetter() ) {
                GetSetter *gs = rcast( var.ptr() );
                type = gs->get_type( scope );
            } else {
                if ( get_res ) *get_res = var;
                type = var.type;
            }
        } else
            return scope->add_error( "to get the type, we need either a typeof_... or a get_..." ), scope->vm->type_Error;
    }

    return type;
}

Variable GetSetter::get_var( Scope *scope, bool rec ) {
    if ( ! var ) {
        if ( ! get )
            return scope->add_error( "we need also a getter for this variable" ), scope->vm->ref_error;
        var = get.apply( scope, true );
    }

    if ( rec && var.type->getsetter() )
        return reinterpret_cast<GetSetter *>( var.ptr() )->get_var( scope, rec );
    if ( type == 0 && var.type->getsetter() == false )
        type = var.type;
    return var;
}
