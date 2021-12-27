#include "gtest/gtest.h"
#include "extraction/BmpExtractor.h"
#include "common/types/Color24bDiff.hpp"

TEST(MatrixTest, checkTruePositives)
{
	am::common::types::Color24b base = {10, 10, 10};
	am::common::types::Color24b compare = {9, 9, 9};

	am::common::types::Color24bDiff diff(base, compare);

	EXPECT_EQ(diff.isPositveR(), true);
	EXPECT_EQ(diff.isPositveG(), true);
	EXPECT_EQ(diff.isPositveB(), true);
}

TEST(MatrixTest, checkFalsePositives)
{
	am::common::types::Color24b base = {10, 10, 10};
	am::common::types::Color24b compare = {20, 20, 20};

	am::common::types::Color24bDiff diff(base, compare);

	EXPECT_EQ(diff.isPositveR(), false);
	EXPECT_EQ(diff.isPositveG(), false);
	EXPECT_EQ(diff.isPositveB(), false);
}

TEST(MatrixTest, checkMixedPositives)
{
	am::common::types::Color24b base = {10, 10, 10};
	am::common::types::Color24b compare = {20, 5, 20};

	am::common::types::Color24bDiff diff(base, compare);
	EXPECT_EQ(diff.isPositveR(), false);
	EXPECT_EQ(diff.isPositveG(), true);
	EXPECT_EQ(diff.isPositveB(), false);

	am::common::types::Color24b compare1 = {5, 15, 5};
	am::common::types::Color24bDiff diff1(base, compare1);
	EXPECT_EQ(diff1.isPositveR(), true);
	EXPECT_EQ(diff1.isPositveG(), false);
	EXPECT_EQ(diff1.isPositveB(), true);
}
