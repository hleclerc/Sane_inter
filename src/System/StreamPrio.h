/*
*/

#pragma once

#include <functional>
#include "Stream.h"

/**

*/
class StreamPrio {
public:
    using TS = std::ostream;

    StreamPrio( TS &stream, int prio, bool close = false ) : stream( stream ), prio( prio ), close( close ) {
        if ( close )
            stream << "( ";
    }

    ~StreamPrio() {
        if ( close )
            stream << " )";
    }

    StreamPrio operator()( int n_prio ) {
        return { stream, n_prio, prio < n_prio };
    }

    template<class T>
    TS &operator<<( const T &val ) {
        return stream << val;
    }

    TS &operator<<( const std::function<void(StreamPrio &)> &func ) {
        func( *this );
        return stream;
    }

    TS  &stream;
    int  prio;
    bool close;
};
