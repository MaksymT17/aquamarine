#pragma once

#include "common/types/Matrix.hpp"
#include "common/types/ColorChannels.hpp"
#include <memory>

namespace am {
	namespace analyze {
		namespace algorithm {

			class ImagePair {
			public:
				ImagePair(std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> base, std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> toCompare);
				virtual ~ImagePair() = default;

				size_t getWidth() const;
				size_t getHeight() const;
				uint16_t getAbsoluteDiff(const size_t rowId, const size_t colId) const;

				std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> getFirst()const;
				std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> getSecond()const;

			private:
				std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> mBase;
				std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> mCompare;

				size_t mWidth;
				size_t mHeight;
			};

		}
	}
}