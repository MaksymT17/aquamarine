#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "Object.h"
#include <vector>

namespace am {
	namespace analyze {
		namespace algorithm {

			enum class MovementType : uint8_t
			{
				NONE = 0,
				LEFT = 1,
				RIGHT = 2,
				TOP = 4,
				BOTTOM = 8
			};

			struct MovementResult
			{
				Object obj;
				MovementType type;
			};

			//* class for detection movement,
			//* needs base image, and set of images to compare
			//* comparison should be done only in area of potential objects
			class IObjectMovementDetector
			{
			public:
				virtual ~IObjectMovementDetector() = default;

				virtual void setBaseImage(std::shared_ptr<am::common::types::Matrix<ColorChannels>> newBase) = 0;

				virtual std::vector<MovementResult> getMovementsResults(std::shared_ptr<ImagePair> pair) = 0;
			};

		}
	}
}