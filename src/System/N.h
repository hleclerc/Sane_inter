#pragma once

/**
*/
template<int n>
struct N {
    enum { val = n };
    operator int() const { return n; }
};
