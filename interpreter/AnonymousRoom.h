#pragma once

#include "System/Stream.h"

struct AnonymousRoom {
    void write_to_stream( std::ostream &os ) const { os << "AnonymousRoom(" << size << "," << alig << ")"; }

    PT   size;
    PT   alig;
};
