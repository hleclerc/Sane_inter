#include "System/Cmp.h"
#include "SurdefList.h"
#include "Lambda.h"
#include "equal.h"
#include "Class.h"
#include "Type.h"
#include "gvm.h"

#include "Inst/(test_known.ts).h"

bool equal( const Variable &a, const Variable &b ) {
    // primitive numbers
    bool res;
    if ( test_known( a, b, [&res]( auto a, auto b ) { res = Cmp<Signed<decltype(a)>::val,Signed<decltype(b)>::val>::my_equ( a, b ); return 1; }, 0 ) )
        return res;

    //
    if ( a.type == gvm->type_Type ) {
        Type *va = *a.rcast<Type *>();
        if ( b.type == gvm->type_SurdefList ) {
            SurdefList *vb = b.rcast<SurdefList>();
            return vb->eq( va );
        } else if ( b.type == gvm->type_Type ) {
            Type *vb = *b.rcast<Type *>( );
            return va == vb;
        } else {
            gvm->add_error( "" );
            P( *b.type );
            P( b );
            TODO;
        }
    }

    if ( a.type == gvm->type_SurdefList ) {
        SurdefList *va = a.rcast<SurdefList>();
        if ( b.type == gvm->type_SurdefList ) {
            SurdefList *vb = b.rcast<SurdefList>();
            return va->eq( vb );
        } else if ( b.type == gvm->type_Type ) {
            Type *vb = *b.rcast<Type *>();
            return va->eq( vb );
        } else {
            P( *b.type );
            P( b );
            TODO;
        }
    }

    //    if ( a.type == gvm->type_AT ) {
    //        AT *av = rcast( a.ptr() );
    //        if ( b.type == gvm->type_NullPtr )
    //            return av->ptr->content == 0;
    //        if ( b.type == gvm->type_AT ) {
    //            AT *bv = rcast( b.ptr() );
    //            return av->ptr->content == bv->ptr->content && av->ptr->offset_in_bytes == bv->ptr->offset_in_bytes;
    //        }
    //        gvm->add_error( "TODO: {} == {}", *a.type, *b.type );
    //        TODO;
    //    }

    if ( a.type == gvm->type_Class ) {
        Class *va = a.rcast<Class>();
        if ( b.type == gvm->type_Class ) {
            Class *vb = b.rcast<Class>();
            return va == vb;
        } else
            TODO;
    }

    if ( a.type == gvm->type_Lambda ) {
        Lambda *av = a.rcast<Lambda>();
        if ( b.type == gvm->type_Lambda ) {
            Lambda *bv = b.rcast<Lambda>();
            return *av == *bv;
        } else
            TODO;
    }

    return a.find_attribute( "operator ==", true, true ).apply( true, b ).is_true();
}
