#pragma once
#include <gmock/gmock.h>
#include "extraction/IMultipleExtractor.h"

namespace am::unit_tests {
    class MockMultipleExtractor : public am::extraction::IMultipleExtractor {
    public:
        MOCK_METHOD(std::vector<am::common::types::Matrix<am::common::types::Color24b>>, readFiles, (std::vector<std::string>&& fileNames), (override));
    };
}
