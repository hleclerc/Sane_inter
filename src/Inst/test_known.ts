const types = [
    "PI64",
    "SI64",
    "PI32",
    "SI32",
    "PI16",
    "SI16",
    "PI8" ,
    "SI8" ,
    "Bool",
    "PT"  ,
    "ST"  ,
];

function so( t: string ): string {
    if ( t == "Bool" ) return "1";
    return `8 * sizeof( ${ t } )`;
}

console.log( `/// generated file
#include "../System/byte_swap.h"

template<class TV,class TR,class OP>
TR test_known( const TV &a, const TV &b, const OP &op, TR dr = {} ) {
    ${ types.map( ta => `if ( a.type == gvm->type_${ ta } ) {
        ${ ta } va;
        if ( a.get_bytes( &va, 0, 0, ${ so( ta ) } ) ) {
            ${ types.map( tb => `if ( b.type == gvm->type_${ tb } ) {
                ${ tb } vb;
                if ( b.get_bytes( &vb, 0, 0, ${ so( tb ) } ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }` ).join( "\n            " ) }
        }
    }` ).join( "\n    " ) }
    return {};
}

template<class TV,class TR,class OP>
TR test_known( const TV &a, const OP &op, TR dr = {} ) {
    ${ types.map( ta => `if ( a.type == gvm->type_${ ta } ) {
        ${ ta } va;
        if ( a.get_bytes( &va, 0, 0, ${ so( ta ) } ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }` ).join( "\n    " ) }
    return {};
}

` );
