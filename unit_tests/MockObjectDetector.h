#pragma once
#include <gmock/gmock.h>
#include "analyze/algorithm/IObjectDetector.h"

namespace am::unit_tests {
    class MockObjectDetector : public am::analyze::algorithm::IObjectDetector {
    public:
        MOCK_METHOD(am::analyze::algorithm::DescObjects, getObjectsRects, (am::analyze::algorithm::ImagePair& pair), (override));
    };
}
