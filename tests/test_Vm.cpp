#include "../src/gvm.h"
#include <gtest/gtest.h>

TEST( Vm, error_if_file_does_not_exist ) {
    gvm = new Vm;
    std::ostringstream ss;
    gvm->error_stream = &ss;

    gvm->import( "murf.foo" );
    ASSERT_TRUE( ss.str().size() );
    ASSERT_TRUE( gvm->error_list.size() );
}