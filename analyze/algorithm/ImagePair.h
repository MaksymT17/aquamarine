#pragma once

#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"
#include <memory>

namespace am {
	namespace analyze {
		namespace algorithm {

			class ImagePair {
			public:
				ImagePair(std::shared_ptr<common::types::Matrix<common::types::Color24b>> base,
					std::shared_ptr<common::types::Matrix<common::types::Color24b>> toCompare);
				virtual ~ImagePair() = default;

				size_t getWidth() const;
				size_t getHeight() const;
				uint16_t getAbsoluteDiff(const size_t rowId, const size_t colId) const;

				std::shared_ptr<common::types::Matrix<common::types::Color24b>> getFirst()const;
				std::shared_ptr<common::types::Matrix<common::types::Color24b>> getSecond()const;

			private:
				std::shared_ptr<common::types::Matrix<common::types::Color24b>> mBase;
				std::shared_ptr<common::types::Matrix<common::types::Color24b>> mCompare;

				size_t mWidth;
				size_t mHeight;
			};

		}
	}
}