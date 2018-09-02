#pragma once

#include<memory>
#include"common/types/Matrix.hpp"
#include"common/types/ColorChannelsDiff.hpp"


namespace am {
	namespace analyze {
		class IComparer
		{
		public:
			virtual ~IComparer() = default;

			virtual std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> compare(std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> newSource) = 0;
		};
	}
}
