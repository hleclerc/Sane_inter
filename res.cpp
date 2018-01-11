#include <stdint.h>
#include "src/System/Stream.h"
#include <unistd.h>

typedef float       FP32;
typedef double      FP64;
typedef long double FP80;
typedef bool        Bool;
typedef int8_t      SI8 ;
typedef int16_t     SI16;
typedef int32_t     SI32;
typedef int64_t     SI64;
typedef uint8_t     PI8 ;
typedef uint16_t    PI16;
typedef uint32_t    PI32;
typedef uint64_t    PI64;
typedef char        Char;
struct Stream {
    SI32 fd;
};
void write_fd( int fd, const char *content, size_t len ) {
    write( fd, content, len );
}
template<class T>
char *itoa( char *buf, T content ) {
    if ( content == 0 ) {
        *( buf++ ) = '0';
        return buf;
    }
    if ( content < 0 ) {
        *( buf++ ) = '-';
        content = - content;
    }
    char *res = buf;
    while( content ) {
        *( res++ ) = '0' + content % 10;
        content /= 10;
    }
    for( char *fub = res, tmp; --fub > buf; ++buf ) {
        tmp = *fub; *fub = *buf; *buf = tmp;
    }
    return res;
}
void write_fd( int fd, const SI32 &content ) {
    char buf[ 11 ];
    char *end = itoa( buf, content );
    write_fd( fd, buf, end - buf );
}

int main( int argc, char **argv ) {
    Stream R0 = {};
    SI32 R1 = {0};
    R0.fd = R1;
    Bool R2 = {};
    SI32 R3 = rand();
    Bool R4 = R3;
    R2 = R4;
    SI32 R5 = {0};
    if ( R2 ) {
        SI32 R6 = {2};
        R5 = R6;
    } else {
        SI32 R6 = {3};
        R5 = R6;
    }
    write_fd(R0.fd,R5);
}
