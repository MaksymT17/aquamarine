#pragma once

#include<memory>
#include"common/Types.hpp"

namespace recognition {
	namespace analyze {
		using namespace common;

		class IComparer
		{
		public:
			virtual ~IComparer() = default;

			virtual std::shared_ptr<Matrix<ColorChannelsDiff>> compare(std::shared_ptr<Matrix<ColorChannels>> newSource) = 0;
		};
	}
}
