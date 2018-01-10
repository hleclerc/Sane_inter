#include "../System/Cmp.h"
#include "../Type.h"
#include "../gvm.h"
#include "UnaOp.h"
#include "Cst.h"

#include "(test_known.ts).h"

namespace {

template<class T> T not_bitwise( T va ) { return ~ va; }
bool not_bitwise( bool va ) { return ! va; }

}

Value make_Neg( const Value &a ) {
    if ( Value r = test_known( a, []( auto va ) { return make_Cst( - va ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Not_logical( const Value &a ) {
    if ( Value r = test_known( a, []( auto va ) { return make_Cst( ! va ); }, Value{} ) )
        return r;
    TODO; return {};
}

Value make_Not_bitwise( const Value &a ) {
    if ( Value r = test_known( a, []( auto va ) { return make_Cst( not_bitwise( va ) ); }, Value{} ) )
        return r;
    TODO; return {};
}
