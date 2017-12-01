#pragma once

#include "TensorOrder.h"
#include "TypeConfig.h"
#include "EnableIf.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

template<class O,class R> struct ShouldBeDisplayedWriteToStream {
    typedef R T;
};

template<class R> struct ShouldBeDisplayedWriteToStream<Bool,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<char,R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<SI8 ,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<SI16,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<SI32,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<SI64,R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<PI8 ,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<PI16,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<PI32,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<PI64,R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<FP32,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<FP64,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<FP80,R> {};

template<class O,class R> struct ShouldBeDisplayedWriteToStream<O *,R> {};
template<ST n,class R> struct ShouldBeDisplayedWriteToStream<char [n],R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<std::string,R> {};


template<class T>
typename EnableIf<
  TensorOrder<T>::res == 0,
  typename ShouldBeDisplayedWriteToStream<T,std::ostream>::T
>::T &operator<<( std::ostream &os, const T &val ) {
    const_cast<T &>( val ).write_to_stream( os );
    return os;
}



template<class T>
typename EnableIf<
  TensorOrder<T>::res == 1,
  std::ostream
>::T &operator<<( std::ostream &os, const T &val ) {
    if ( size_t s = val.size() ) {
        os << val[ 0 ];
        for( size_t i = 1; i < s; ++i )
            os << ' ' << val[ i ];
    }
    return os;
}

template<class OS,class T0> void __my_print( OS &os, const T0 &t0 ) { os << t0 << std::endl; }
template<class OS,class T0,class... Args> void __my_print( OS &os, const T0 &t0, const Args &...args ) { os << t0 << ", "; __my_print( os, args... ); }

#ifndef P
    #define P( ... ) \
        __my_print( std::cout << #__VA_ARGS__ " -> ", __VA_ARGS__ );
    #define I( N, ... ) \
        __my_print( std::cout << N " -> ", __VA_ARGS__ );
    #define PE( ... ) \
        __my_print( std::cerr << #__VA_ARGS__ " -> ", __VA_ARGS__ );
    #define PM( ... ) \
        __my_print( std::cout << __FILE__ << ':' << __LINE__ << ": "  << #__VA_ARGS__ " -> ", __VA_ARGS__ );
    #define PN( ... ) \
        __my_print( std::cout << #__VA_ARGS__ " ->\n", __VA_ARGS__ );
    #define PC do { static int cpt = 0; PE( cpt++ ); } while ( false )
    #define PS( val ) static int cpt = 0; if ( cpt++ == val )
#endif

template<class T>
std::string to_string( const T &val ) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

std::string to_string( const PI8 *val );

