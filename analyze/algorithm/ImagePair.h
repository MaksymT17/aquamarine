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

				size_t getWidth() const noexcept;
				size_t getHeight() const noexcept;
				uint16_t getAbsoluteDiff(const size_t rowId, const size_t colId) const noexcept;

				std::shared_ptr<common::types::Matrix<common::types::Color24b>> getFirst() const noexcept;
				std::shared_ptr<common::types::Matrix<common::types::Color24b>> getSecond() const noexcept;

			private:
				std::shared_ptr<common::types::Matrix<common::types::Color24b>> mBase;
				std::shared_ptr<common::types::Matrix<common::types::Color24b>> mCompare;
			};

		}
	}
}