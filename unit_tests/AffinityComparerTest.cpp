#include "gtest/gtest.h"
#include "analyze/AffinityComparer.h"

using namespace am::analyze;
using namespace am::common::types;
TEST(AffinityComparer, check2x2)
{
    am::common::types::Matrix<am::common::types::Color24b> base(2,2);
    am::common::types::Color24b color{201,202,203};
    base(0,0)=color;
    base(0,1)=color;
    base(1,0)=color;
    base(1,1)=color;
    
    am::common::types::Matrix<am::common::types::Color24b> compare(2,2);
    am::common::types::Color24b color1{100,100,100};
    compare(0,0)=color1;
    compare(0,1)=color1;
    compare(1,0)=color;
    compare(1,1)=color1;

    
    Matrix<Color24bDiff> res = AffinityComparer::compare(base, compare, 2);

    EXPECT_EQ(res(1,0).r, 0);
    EXPECT_EQ(res(1,0).g, 0);
    EXPECT_EQ(res(1,0).b, 0);

    EXPECT_EQ(res(1,1).r, 101);
    EXPECT_EQ(res(1,1).g, 102);
    EXPECT_EQ(res(1,1).b, 103);

    EXPECT_EQ(res(0,1).r, 101);
    EXPECT_EQ(res(0,1).g, 102);
    EXPECT_EQ(res(0,1).b, 103);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}