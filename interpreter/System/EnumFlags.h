#pragma once

#include <type_traits>

template<typename T>
constexpr typename std::underlying_type<T>::type underlying( T t ) { return typename std::underlying_type<T>::type( t ); }

template<typename T>
struct EnumOrBool {
    constexpr EnumOrBool( T t ) : t( t ) {}

    constexpr          operator T   () const { return t; }
    constexpr explicit operator bool() const { return underlying( t ); }

    T t;
};

#define ENUM_FLAGS( T ) \
    inline constexpr EnumOrBool<T> operator & ( T  x, T y ) { return T(  underlying( x ) & underlying( y ) ); } \
    inline constexpr T             operator | ( T  x, T y ) { return T(  underlying( x ) | underlying( y ) ); } \
    inline constexpr T             operator ^ ( T  x, T y ) { return T(  underlying( x ) ^ underlying( y ) ); } \
    inline constexpr T             operator ~ ( T  x      ) { return T( ~underlying( x ) ); } \
    inline T                      &operator &=( T &x, T y ) { x = x & y; return x; } \
    inline T                      &operator |=( T &x, T y ) { x = x | y; return x; } \
    inline T                      &operator ^=( T &x, T y ) { x = x ^ y; return x; } \

