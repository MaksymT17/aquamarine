#pragma once
#include"IComparer.h"
#include "common/types/ColorChannelsDiff.hpp"
#include "common/types/Matrix.hpp"

namespace aq
{
	namespace analyze
	{
		enum DataMode
		{
			KEEP_BASE_FRAME = 0,     // keep base frame for multiple comparisons
			SWITCH_TO_COMPARED = 1   // after every comparison base frame will replace to new(compared last)
		};

		using namespace common::types;
		class AffinityComparer : public IComparer
		{
		public:
			AffinityComparer(std::shared_ptr<Matrix<ColorChannels>> base);

			virtual ~AffinityComparer() = default;

			virtual std::shared_ptr<common::types::Matrix<ColorChannelsDiff>> compare(std::shared_ptr<common::types::Matrix<ColorChannels>> newSource) override;

			bool isRequestSizeValid(std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> newSource) const;

		private:
			std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> mBase;
			DataMode mMode;
		};
	}
}
