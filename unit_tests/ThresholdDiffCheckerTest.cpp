#include "gtest/gtest.h"
#include "analyze/ThresholdDiffChecker.h"
#include "extraction/BmpExtractor.h"
#include "common/types/Color24bDiff.hpp"
#include "analyze/AffinityComparer.h"

using namespace am::analyze;
using namespace am::common::types;
TEST(ThresholdDiffCheckerTest, check75percent_2x2)
{
    ThresholdDiffChecker thr(100);
    ThresholdDiffChecker thr_false(304); // shall not find a difference, threshold is too big
    am::common::types::Matrix<am::common::types::Color24b> base(2, 2);
    am::common::types::Color24b color{201, 201, 201};
    base(0, 0) = color;
    base(0, 1) = color;
    base(1, 0) = color;
    base(1, 1) = color;

    // only 1 pixel out of 4 is set with different color
    // expected 1/4 = 25% of the affinity percent
    am::common::types::Matrix<am::common::types::Color24b> compare(2, 2);
    am::common::types::Color24b color1{100, 100, 100};
    compare(0, 0) = color1;
    compare(0, 1) = color1;
    compare(1, 0) = color;
    compare(1, 1) = color1;

    Matrix<Color24bDiff> res = AffinityComparer::compare(base, compare, 4);

    EXPECT_EQ(thr.getAffinityPersent(4, res), 0.25f);
    EXPECT_EQ(thr_false.getAffinityPersent(4, res), 1.00f);

    // check comparison by pixels
    auto diff = ThresholdDiffChecker::getThresholdDiff(res, 4, 100);
    EXPECT_EQ(diff(0, 0), am::common::CHANGE);
    EXPECT_EQ(diff(0, 1), am::common::CHANGE);
    EXPECT_EQ(diff(1, 0), 0);
    EXPECT_EQ(diff(1, 1), am::common::CHANGE);
}

TEST(ThresholdDiffCheckerTest, check75percent_3x3)
{
    ThresholdDiffChecker thr(1);

    am::common::types::Matrix<am::common::types::Color24b> base(4, 4);
    am::common::types::Color24b color{201, 201, 201};
    am::common::types::Color24b color1{200, 200, 200};

    for (size_t y = 0; y < 4; ++y) {
        for (size_t x = 0; x < 4; ++x) {
            base(y, x) = color;
        }
    }

    // only 4 pixel out of 16 are set with different color
    // expected 1/4 = 75% of the affinity percent
    am::common::types::Matrix<am::common::types::Color24b> compare(4, 4);
    for (size_t y = 0; y < 4; ++y) {
        for (size_t x = 0; x < 4; ++x) {
            compare(y, x) = color;
        }
    }
    
    compare(0, 3) = color1;
    compare(1, 0) = color1;
    compare(2, 0) = color1;
    compare(2, 1) = color1;

    Matrix<Color24bDiff> res = AffinityComparer::compare(base, compare, 1);

    EXPECT_TRUE(thr.getAffinityPersent(4, res) == 0.75f);
}
