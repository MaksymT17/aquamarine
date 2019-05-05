#pragma once

#include<memory>
#include"common/types/Matrix.hpp"
#include"common/types/Color24bDiff.hpp"

namespace am {
	namespace analyze {
		class IComparer
		{
		public:
			virtual ~IComparer() = default;

			virtual std::shared_ptr<common::types::Matrix<common::types::Color24bDiff>> compare(std::shared_ptr<common::types::Matrix<common::types::Color24b>> newSource) = 0;
		};
	}
}
