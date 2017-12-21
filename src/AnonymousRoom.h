#pragma once

#include "System/Stream.h"

struct AnonymousRoom {
    AnonymousRoom( SI32 size, SI32 alig ) : size( size ), alig( alig ) {
    }

    void write_to_stream( std::ostream &os ) const { os << "AnonymousRoom(" << size << "," << alig << ")"; }

    SI32 size;
    SI32 alig;
};
