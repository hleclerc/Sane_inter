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
    "Char",
    "PT"  ,
    "ST"  ,
];

function so( t: string ): string {
    if ( t == "Bool" ) return "1";
    return `8 * sizeof( ${ t } )`;
}

function rc( t: string, n: string ): string {
    // if ( t == "Bool")
    //     return `BoolRef v${ n }( ${ n }.content->data, ${ n }.off )`
    return `${ t } &v${ n } = *reinterpret_cast<${ t } *>( ${ n }.ptr() )`
}

console.log( `/// file generated
template<class TR,class OP>
TR test_known( Vm *vm, const Variable &a, const Variable &b, const OP &op, TR dr = {} ) {
    ${ types.map( ta => `if ( a.type == vm->type_${ ta } ) {
        ${ rc( ta, "a" ) };
        ${ types.map( tb => `if ( b.type == vm->type_${ tb } ) {
            ${ rc( tb, "b" ) };
            return op( va, vb );
        }` ).join( "\n        " ) }
    }` ).join( "\n    " ) }
    return dr;
}

template<class TR,class OP>
TR test_known( Vm *vm, const Variable &a, const OP &op, TR dr = {} ) {
    ${ types.map( ta => `if ( a.type == vm->type_${ ta } ) {
        ${ rc( ta, "a" ) };
        return op( va );
    }` ).join( "\n    " ) }
    return dr;
}

` );
