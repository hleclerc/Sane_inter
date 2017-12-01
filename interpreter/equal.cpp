#include "System/rcast.h"
#include "SurdefList.h"
#include "Lambda.h"
#include "Scope.h"
#include "equal.h"
#include "Class.h"
#include "Cmp.h"
#include "AT.h"
#include "Vm.h"

#include "(test_known.ts).h"


bool equal( Scope *scope, const Variable &a, const Variable &b ) {
    if ( a.type->getsetter() ) return equal( scope, a.ugs( scope ), b );
    if ( b.type->getsetter() ) return equal( scope, a, b.ugs( scope ) );

    // primitive numbers
    bool res;
    if ( test_known( scope->vm, a, b, [&res]( auto a, auto b ) { res = Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_equ( a, b ); return 1; }, 0 ) )
        return res;

    //
    if ( a.type == scope->vm->type_Type ) {
        Type *va = *reinterpret_cast<Type **>( a.ptr() );
        if ( b.type == scope->vm->type_SurdefList ) {
            SurdefList *vb = rcast( b.ptr() );
            return vb->eq( scope, va );
        } else if ( b.type == scope->vm->type_Type ) {
            Type *vb = *reinterpret_cast<Type **>( b.ptr() );
            return va == vb;
        } else {
            scope->add_error("");
            P( b );
            P( *b.type );
            TODO;
        }
    }

    if ( a.type == scope->vm->type_SurdefList ) {
        SurdefList *va = rcast( a.ptr() );
        if ( b.type == scope->vm->type_SurdefList ) {
            SurdefList *vb = rcast( b.ptr() );
            return va->eq( scope, vb );
        } else if ( b.type == scope->vm->type_Type ) {
            Type *vb = *reinterpret_cast<Type **>( b.ptr() );
            return va->eq( scope, vb );
        } else {
            P( b );
            P( *b.type );
            TODO;
        }
    }

    if ( a.type == scope->vm->type_AT ) {
        AT *av = rcast( a.ptr() );
        if ( b.type == scope->vm->type_NullPtr )
            return av->ptr->content == 0;
        if ( b.type == scope->vm->type_AT ) {
            AT *bv = rcast( b.ptr() );
            return av->ptr->content == bv->ptr->content && av->ptr->offset_in_bytes == bv->ptr->offset_in_bytes;
        }
        scope->add_error( "TODO: {} == {}", *a.type, *b.type );
        TODO;
    }

    if ( a.type == scope->vm->type_Class ) {
        Class *va = rcast( a.ptr() );
        if ( b.type == scope->vm->type_Class ) {
            Class *vb = rcast( b.ptr() );
            return va == vb;
        } else
            TODO;
    }

    if ( a.type == scope->vm->type_Lambda ) {
        Lambda *av = rcast( a.ptr() );
        if ( b.type == scope->vm->type_Lambda ) {
            Lambda *bv = rcast( b.ptr() );
            return *av == *bv;
        } else
            TODO;
    }

    return a.find_attribute( scope, "operator ==", true, true ).apply( scope, true, b ).is_true( scope );
}
