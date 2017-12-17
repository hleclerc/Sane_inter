#pragma once

#include "Stream.h"

/**
*/
class Bool_vec {
public:
    Bool_vec( Reference, void *data, size_t size );
    Bool_vec( size_t size, bool val ); ///< owned data
    Bool_vec( size_t size ); ///< owned data
    ~Bool_vec();

    void   write_to_stream( std::ostream &os ) const;

    bool   all_true () const; ///< true is all bits are set
    bool   all_false() const; ///< true is all bits are not set

    bool   all_true ( size_t beg, size_t end ) const; ///< true is all bits are set
    bool   all_false( size_t beg, size_t end ) const; ///< true is all bits are not set

    void   set      ( size_t index, bool val );
    bool   get      ( size_t index ) const;

    void   set      ( size_t beg, size_t end, bool val );

    PI8   *data;
    size_t size;
    size_t rese;
};
