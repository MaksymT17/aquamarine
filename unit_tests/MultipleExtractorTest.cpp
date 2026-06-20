#include "gtest/gtest.h"
#include "extraction/MultipleExtractor.h"
#include "extraction/ExtractorFactory.h"

using namespace am::common::types;

TEST(MultipleExtractorTest, extractMultipleFiles)
{
    auto factory = std::make_unique<am::extraction::ExtractorFactory>();
    factory->registerExtractor("test", [](const std::string& path) {
        return Matrix<Color24b>(10, 10);
    });

    am::extraction::MultipleExtractor multipleExtractor(std::move(factory));

    std::vector<std::string> files = {"file1.test", "file2.test", "file3.test"};
    auto result = multipleExtractor.readFiles(std::move(files));

    ASSERT_EQ(result.size(), 3);
    for (const auto& mat : result) {
        EXPECT_EQ(mat.getWidth(), 10);
        EXPECT_EQ(mat.getHeight(), 10);
    }
}
