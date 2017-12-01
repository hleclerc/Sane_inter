/// file generated
template<class TR,class OP>
TR test_known( Vm *vm, const Variable &a, const Variable &b, const OP &op, TR dr = {} ) {
    if ( a.type == vm->type_PI64 ) {
        PI64 &va = *reinterpret_cast<PI64 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_SI64 ) {
        SI64 &va = *reinterpret_cast<SI64 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_PI32 ) {
        PI32 &va = *reinterpret_cast<PI32 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_SI32 ) {
        SI32 &va = *reinterpret_cast<SI32 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_PI16 ) {
        PI16 &va = *reinterpret_cast<PI16 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_SI16 ) {
        SI16 &va = *reinterpret_cast<SI16 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_PI8 ) {
        PI8 &va = *reinterpret_cast<PI8 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_SI8 ) {
        SI8 &va = *reinterpret_cast<SI8 *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_Bool ) {
        Bool &va = *reinterpret_cast<Bool *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_Char ) {
        Char &va = *reinterpret_cast<Char *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_PT ) {
        PT &va = *reinterpret_cast<PT *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    if ( a.type == vm->type_ST ) {
        ST &va = *reinterpret_cast<ST *>( a.ptr() );
        if ( b.type == vm->type_PI64 ) {
            PI64 &vb = *reinterpret_cast<PI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI64 ) {
            SI64 &vb = *reinterpret_cast<SI64 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI32 ) {
            PI32 &vb = *reinterpret_cast<PI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI32 ) {
            SI32 &vb = *reinterpret_cast<SI32 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI16 ) {
            PI16 &vb = *reinterpret_cast<PI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI16 ) {
            SI16 &vb = *reinterpret_cast<SI16 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PI8 ) {
            PI8 &vb = *reinterpret_cast<PI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_SI8 ) {
            SI8 &vb = *reinterpret_cast<SI8 *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Bool ) {
            Bool &vb = *reinterpret_cast<Bool *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_Char ) {
            Char &vb = *reinterpret_cast<Char *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_PT ) {
            PT &vb = *reinterpret_cast<PT *>( b.ptr() );
            return op( va, vb );
        }
        if ( b.type == vm->type_ST ) {
            ST &vb = *reinterpret_cast<ST *>( b.ptr() );
            return op( va, vb );
        }
    }
    return dr;
}

template<class TR,class OP>
TR test_known( Vm *vm, const Variable &a, const OP &op, TR dr = {} ) {
    if ( a.type == vm->type_PI64 ) {
        PI64 &va = *reinterpret_cast<PI64 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_SI64 ) {
        SI64 &va = *reinterpret_cast<SI64 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_PI32 ) {
        PI32 &va = *reinterpret_cast<PI32 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_SI32 ) {
        SI32 &va = *reinterpret_cast<SI32 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_PI16 ) {
        PI16 &va = *reinterpret_cast<PI16 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_SI16 ) {
        SI16 &va = *reinterpret_cast<SI16 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_PI8 ) {
        PI8 &va = *reinterpret_cast<PI8 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_SI8 ) {
        SI8 &va = *reinterpret_cast<SI8 *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_Bool ) {
        Bool &va = *reinterpret_cast<Bool *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_Char ) {
        Char &va = *reinterpret_cast<Char *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_PT ) {
        PT &va = *reinterpret_cast<PT *>( a.ptr() );
        return op( va );
    }
    if ( a.type == vm->type_ST ) {
        ST &va = *reinterpret_cast<ST *>( a.ptr() );
        return op( va );
    }
    return dr;
}


