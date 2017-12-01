#pragma once

#include "System/TypeConfig.h"

template<class T> struct Signed { enum { val = false }; using U = T; };
template<> struct Signed<SI8 > { enum { val = true }; using U = PI8 ; };
template<> struct Signed<SI16> { enum { val = true }; using U = PI16; };
template<> struct Signed<SI32> { enum { val = true }; using U = PI32; };
template<> struct Signed<SI64> { enum { val = true }; using U = PI64; };
template<> struct Signed<Char> { enum { val = true }; using U = unsigned char; };

template<bool sa,bool sb>
struct Cmp {
    template<class A,class B> static bool my_equ    ( A a, B b ) { return a == b; }
    template<class A,class B> static bool my_not_equ( A a, B b ) { return a != b; }
    template<class A,class B> static bool my_inf    ( A a, B b ) { return a <  b; }
    template<class A,class B> static bool my_inf_equ( A a, B b ) { return a <= b; }
    template<class A,class B> static bool my_sup    ( A a, B b ) { return a >  b; }
    template<class A,class B> static bool my_sup_equ( A a, B b ) { return a >= b; }
};

template<>
struct Cmp<true,false> {
    template<class A,class B> static bool my_equ    ( A a, B b ) { return a >= 0 && typename Signed<A>::U( a ) == b; }
    template<class A,class B> static bool my_not_equ( A a, B b ) { return a <  0 || typename Signed<A>::U( a ) != b; }
    template<class A,class B> static bool my_inf    ( A a, B b ) { return a <  0 || typename Signed<A>::U( a ) <  b; }
    template<class A,class B> static bool my_inf_equ( A a, B b ) { return a <  0 || typename Signed<A>::U( a ) <= b; }
    template<class A,class B> static bool my_sup    ( A a, B b ) { return a >  0 && typename Signed<A>::U( a ) >  b; }
    template<class A,class B> static bool my_sup_equ( A a, B b ) { return a >= 0 && typename Signed<A>::U( a ) >= b; }
};

template<>
struct Cmp<false,true> {
    template<class A,class B> static bool my_equ    ( A a, B b ) { return b >= 0 && a == typename Signed<B>::U( b ); }
    template<class A,class B> static bool my_not_equ( A a, B b ) { return b <  0 || a != typename Signed<B>::U( b ); }
    template<class A,class B> static bool my_inf    ( A a, B b ) { return b >  0 && a <  typename Signed<B>::U( b ); }
    template<class A,class B> static bool my_inf_equ( A a, B b ) { return b >= 0 && a <= typename Signed<B>::U( b ); }
    template<class A,class B> static bool my_sup    ( A a, B b ) { return b <  0 || a >  typename Signed<B>::U( b ); }
    template<class A,class B> static bool my_sup_equ( A a, B b ) { return b <  0 || a >= typename Signed<B>::U( b ); }
};
