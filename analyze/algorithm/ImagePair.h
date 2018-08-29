#pragma once

#include "common/Types.hpp"
#include <memory>

namespace recognition {
	namespace analyze {
		namespace algorithm {

			class ImagePair {
			public:
				ImagePair(std::shared_ptr<common::Matrix<common::ColorChannels>> base, std::shared_ptr<common::Matrix<common::ColorChannels>> toCompare);
				virtual ~ImagePair() = default;

				size_t getWidth() const;
				size_t getHeight() const;
				uint16_t getAbsoluteDiff(const size_t rowId, const size_t colId) const;

			private:
				std::shared_ptr<common::Matrix<common::ColorChannels>> mBase;
				std::shared_ptr<common::Matrix<common::ColorChannels>> mCompare;

				size_t mWidth;
				size_t mHeight;
			};

		}
	}
}