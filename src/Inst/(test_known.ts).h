/// generated file
#include "../System/byte_swap.h"

template<class TV,class TR,class OP>
TR test_known( const TV &a, const TV &b, const OP &op, TR dr = {} ) {
    if ( a.type == gvm->type_PI64 ) {
        PI64 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI64 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_SI64 ) {
        SI64 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI64 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_PI32 ) {
        PI32 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI32 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_SI32 ) {
        SI32 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI32 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_PI16 ) {
        PI16 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI16 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_SI16 ) {
        SI16 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI16 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_PI8 ) {
        PI8 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI8 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_SI8 ) {
        SI8 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI8 ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_Bool ) {
        Bool va;
        if ( a.get_bytes( &va, 0, 0, 1 ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_PT ) {
        PT va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PT ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    if ( a.type == gvm->type_ST ) {
        ST va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( ST ) ) ) {
            if ( b.type == gvm->type_PI64 ) {
                PI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI64 ) {
                SI64 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI64 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI32 ) {
                PI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI32 ) {
                SI32 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI32 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI16 ) {
                PI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI16 ) {
                SI16 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI16 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PI8 ) {
                PI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_SI8 ) {
                SI8 vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( SI8 ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_Bool ) {
                Bool vb;
                if ( b.get_bytes( &vb, 0, 0, 1 ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_PT ) {
                PT vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( PT ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
            if ( b.type == gvm->type_ST ) {
                ST vb;
                if ( b.get_bytes( &vb, 0, 0, 8 * sizeof( ST ) ) )
                    return gvm->reverse_endianness ? op( byte_swaped( va ), byte_swaped( vb ) ) : op( va, vb );
            }
        }
    }
    return {};
}

template<class TV,class TR,class OP>
TR test_known( const TV &a, const OP &op, TR dr = {} ) {
    if ( a.type == gvm->type_PI64 ) {
        PI64 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI64 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_SI64 ) {
        SI64 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI64 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_PI32 ) {
        PI32 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI32 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_SI32 ) {
        SI32 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI32 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_PI16 ) {
        PI16 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI16 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_SI16 ) {
        SI16 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI16 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_PI8 ) {
        PI8 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PI8 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_SI8 ) {
        SI8 va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( SI8 ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_Bool ) {
        Bool va;
        if ( a.get_bytes( &va, 0, 0, 1 ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_PT ) {
        PT va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( PT ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    if ( a.type == gvm->type_ST ) {
        ST va;
        if ( a.get_bytes( &va, 0, 0, 8 * sizeof( ST ) ) )
            return gvm->reverse_endianness ? op( byte_swaped( va ) ) : op( va );
    }
    return {};
}


