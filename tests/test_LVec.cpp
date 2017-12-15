#include "../src/System/LVec.h"
#include <gtest/gtest.h>

TEST( LVec, init ) {
    LVec<int> v( 1, 2, 3 );

    ASSERT_TRUE( v.size() == 3 );
    ASSERT_TRUE( v[ 0 ] == 1 );
    ASSERT_TRUE( v[ 2 ] == 3 );

    v.push_back( 4 );
    ASSERT_TRUE( v.size() == 4 );
    ASSERT_TRUE( v[ 3 ] == 4 );
}
