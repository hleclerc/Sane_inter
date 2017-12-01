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
];

console.log( `/// generated file
bool reassign_bt( Vm *vm, const Variable &a, const Variable &b ) {
    Type *ta = a.type, *tb = b.type;
    ${ types.map( ta => `if ( ta == vm->type_${ ta } ) {
        ${ ta } *va = rcast( a.ptr() );
        ${ types.map( tb => `if ( tb == vm->type_${ tb } ) {
            ${ tb } *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }` ).join( "\n        " ) }
        return false;
    }` ).join( "\n    " ) }
    return false;
}
` );
