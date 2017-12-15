#include "../src/System/LString.h"
#include <gtest/gtest.h>

TEST( LString, init ) {
    LString v;

    ASSERT_TRUE( v.size() == 0 );
    ASSERT_TRUE( v.empty() );

    LString u = "123";
    ASSERT_TRUE( u.size() == 3 );
    ASSERT_TRUE( u.empty() == false );
    ASSERT_TRUE( u == "123" );
    ASSERT_TRUE( u != "121" );
}
