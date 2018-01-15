#pragma once

#include "Value.h"


/**
*/
class RessourceState {
public:
    void        write_to_stream( std::ostream &os ) const { os << "lw: " << last_writer << " lr: [" << last_readers << "]"; }
    bool        operator==     ( const RessourceState &that ) const { return last_readers == that.last_readers && last_writer == that.last_writer; }
    Value       make_single_inst    () const { if ( last_readers.empty() ) return last_writer; TODO; return {}; }

    Vec<Value>  last_readers;
    Value       last_writer;
};

