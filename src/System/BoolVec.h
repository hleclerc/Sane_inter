#pragma once

#include "Stream.h"

/**
*/
class BoolVec {
public:
    BoolVec( Reference, void *data, size_t size );
    BoolVec( size_t size, bool val ); ///< owned data
    BoolVec( size_t size ); ///< owned data
    ~BoolVec();

    void   write_to_stream( std::ostream &os ) const;

    bool   operator==     ( const BoolVec &that ) const;
    bool   operator!=     ( const BoolVec &that ) const;

    bool   all_true       () const; ///< true if all bits are set
    bool   all_false      () const; ///< true if all bits are not set

    bool   all_true       ( size_t beg, size_t end ) const; ///< true is all bits are set
    bool   all_false      ( size_t beg, size_t end ) const; ///< true is all bits are not set

    bool   get            ( size_t index ) const;

    void   set            ( size_t beg, size_t end, bool val );
    void   set            ( size_t index, bool val );
    void   set            ( bool val );

    PI8   *data;
    size_t size;
    size_t rese;
};
