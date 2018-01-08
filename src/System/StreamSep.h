/*
*/

#pragma once

#include "Stream.h"

/**

*/
class StreamSepRunner {
public:
    using TS = std::ostream;

    StreamSepRunner( TS *stream, const std::string &sep, const std::string &end ) : stream( stream ), sep( sep ), end( end ) {}
    ~StreamSepRunner() { *stream << end; }

    ///
    template<class T>
    StreamSepRunner &operator<<( const T &val ) {
        *stream << sep << val;
        return *this;
    }

    TS         *stream;
    std::string sep;
    std::string end;
};


/**

*/
class StreamSep {
public:
    using TS = std::ostream;

    StreamSep( TS &stream, const std::string &beg = {}, const std::string &sep = {}, const std::string &end = "\n", const std::string &first_beg = {} ) : stream( &stream ), first_beg( first_beg ), beg( beg ), sep( sep ), end( end ) {}
    StreamSep( StreamSep &ss, const std::string &add_beg = {} ) : stream( ss.stream ), first_beg( ss.first_beg ), beg( ss.beg + add_beg ), sep( ss.sep ), end( ss.end ) {}

    ///
    template<class T>
    StreamSepRunner operator<<( const T &val ) {
        if( first_beg.size() ) {
            *stream << first_beg;
            first_beg.clear();
        } else
            *stream << beg;
        *stream << val;
        return StreamSepRunner( stream, sep, end );
    }

    ///
    StreamSep rm_beg( unsigned n ) {
        StreamSep res = *this;
        res.beg.resize( beg.size() - std::min( unsigned( beg.size() ), n ) );
        return res;
    }

    void inc( int n = 4 ) {
        while( n-- )
            beg += ' ';
    }

    void dec( int n = 4 ) {
        beg.resize( beg.size() - n );
    }

    TS &write_beg() {
        if( first_beg.size() ) {
            *stream << first_beg;
            first_beg.clear();
        } else
            *stream << beg;
        return *stream;
    }

    template<class T>
    TS &write_beg( const T &add ) {
        return write_beg() << add;
    }

    TS &write_end() {
        *stream << end;
        return *stream;
    }

    template<class T>
    TS &write_end( const T &add ) {
        *stream << add << end;
        return *stream;
    }

    TS &operator*() { return *stream; }

    TS         *stream;
    std::string first_beg;
    std::string beg;
    std::string sep;
    std::string end;
};
