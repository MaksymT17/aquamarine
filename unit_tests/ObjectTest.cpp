#include "gtest/gtest.h"
#include "analyze/algorithm/Object.h"

using namespace am::analyze::algorithm;

TEST(ObjectTest, check3Pixels)
{
	std::vector<Pixel> pixels{{5,5},{10,10},{20,20}};
	Object obj(pixels);

	EXPECT_EQ(obj.getLeft(), 5);
	EXPECT_EQ(obj.getRight(), 20);

	EXPECT_EQ(obj.getMinHeight(), 5);
	EXPECT_EQ(obj.getMaxHeight(), 20);
	EXPECT_EQ(obj.getPixelsCount(), 3);
}

TEST(ObjectTest, check10Pixels)
{
	std::vector<Pixel> pixels{{5,5},{10,10},{20,20},{6,6},{7,7},
	{11,11},{3,3},{12,6},{13,7},{11,99}};
	Object obj(pixels);

	EXPECT_EQ(obj.getLeft(), 3);
	EXPECT_EQ(obj.getRight(), 99);

	EXPECT_EQ(obj.getMinHeight(), 3);
	EXPECT_EQ(obj.getMaxHeight(), 20);
	EXPECT_EQ(obj.getPixelsCount(), 10);
}

TEST(ObjectTest, check15Pixels)
{
	std::vector<Pixel> pixels{{5,5},{10,10},{20,20},{6,6},{7,7},
	{11,11},{3,3},{12,6},{13,7},{11,99},
	{2,100},{3,33},{5,6},{13,17},{14,9}};
	Object obj(pixels);

	EXPECT_EQ(obj.getLeft(), 3);
	EXPECT_EQ(obj.getRight(), 100);

	EXPECT_EQ(obj.getMinHeight(), 2);
	EXPECT_EQ(obj.getMaxHeight(), 20);
	EXPECT_EQ(obj.getPixelsCount(), 15);
}

TEST(ObjectTest, checkIsMergable)
{
	std::vector<Pixel> pixels{{5,5}};
	Object obj(pixels);

	std::vector<Pixel> new_pixels{{5,6}};
	Object new_obj(new_pixels);
	
	EXPECT_TRUE(new_obj.isMeargableToLeft(obj));
	EXPECT_NO_THROW(new_obj.mergeIfPossibleLeftToMe(obj));

	EXPECT_EQ(new_obj.getLeft(), 5);
	EXPECT_EQ(new_obj.getRight(), 6);
	EXPECT_EQ(new_obj.getMinHeight(), 5);
	EXPECT_EQ(new_obj.getMaxHeight(), 5);
	EXPECT_EQ(new_obj.getPixelsCount(), 2);

	obj.clearPixelsCount();
	EXPECT_EQ(obj.getPixelsCount(), 0);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
