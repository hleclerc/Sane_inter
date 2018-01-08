/*
*/

#pragma once

#include "Stream.h"

/**

*/
class StreamPrioRunner {
public:
    using TS = std::ostream;

    StreamPrioRunner( TS &stream, bool close ) : stream( stream ), close( close ) {
        if( close )
            stream << "( ";
    }

    ~StreamPrioRunner() {
        if ( close )
            stream << " )";
    }

    template<class T>
    StreamPrioRunner &operator<<( const T &val ) {
        stream << val;
        return *this;
    }

    TS  &stream;
    bool close;
};


/**

*/
class StreamPrio {
public:
    using TS = std::ostream;

    StreamPrio( TS &stream, int prio ) : stream( stream ), prio( prio ) {}

    StreamPrioRunner operator()( int n_prio ) {
        return { stream, prio < n_prio };
    }

    template<class T>
    TS &operator<<( const T &val ) {
        return stream << val;
    }

    TS &stream;
    int prio;
};
