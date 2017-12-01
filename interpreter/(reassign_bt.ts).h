/// generated file
bool reassign_bt( Vm *vm, const Variable &a, const Variable &b ) {
    Type *ta = a.type, *tb = b.type;
    if ( ta == vm->type_PI64 ) {
        PI64 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_SI64 ) {
        SI64 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_PI32 ) {
        PI32 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_SI32 ) {
        SI32 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_PI16 ) {
        PI16 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_SI16 ) {
        SI16 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_PI8 ) {
        PI8 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_SI8 ) {
        SI8 *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_Bool ) {
        Bool *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    if ( ta == vm->type_PT ) {
        PT *va = rcast( a.ptr() );
        if ( tb == vm->type_PI64 ) {
            PI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI64 ) {
            SI64 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI32 ) {
            PI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI32 ) {
            SI32 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI16 ) {
            PI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI16 ) {
            SI16 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PI8 ) {
            PI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_SI8 ) {
            SI8 *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_Bool ) {
            Bool *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        if ( tb == vm->type_PT ) {
            PT *vb = rcast( b.ptr() );
            *va = *vb;
            return true;
        }
        return false;
    }
    return false;
}

