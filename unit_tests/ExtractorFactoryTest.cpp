#include "gtest/gtest.h"
#include "extraction/ExtractorFactory.h"
#include "common/exceptions/WrongFormatException.hpp"

using namespace am::common::types;

TEST(ExtractorFactoryTest, registerAndGetExtractor)
{
    am::extraction::ExtractorFactory factory;
    auto mockExtractor = [](const std::string& path) {
        return Matrix<Color24b>(10, 10);
    };

    factory.registerExtractor("dummy", mockExtractor);

    auto retrievedExtractor = factory.getExtractor("dummy");
    ASSERT_TRUE(retrievedExtractor != nullptr);

    auto result = retrievedExtractor("some_path");
    EXPECT_EQ(result.getWidth(), 10);
    EXPECT_EQ(result.getHeight(), 10);
}

TEST(ExtractorFactoryTest, getNonExistingExtractorThrows)
{
    am::extraction::ExtractorFactory factory;

    EXPECT_THROW(factory.getExtractor("non_existing"), am::common::exceptions::WrongFormatException);
}
