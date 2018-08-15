#pragma once
#include"IComparer.h"

namespace recognition
{
	namespace analyze
	{
		enum DataMode
		{
			KEEP_BASE_FRAME = 0,     // keep base frame for multiple comparisons
			SWITCH_TO_COMPARED = 1   // after every comparison base frame will replace to new(compared last)
		};

		using namespace common;
		class AffinityComparer : public IComparer
		{
		public:
			AffinityComparer(std::shared_ptr<Matrix<ColorChannels>> base);

			virtual ~AffinityComparer() = default;

			virtual std::shared_ptr<Matrix<ColorChannelsDiff>> compare(std::shared_ptr<Matrix<ColorChannels>> newSource) override;

			bool isRequestSizeValid(std::shared_ptr<Matrix<ColorChannels>> newSource) const;

		private:
			std::shared_ptr<Matrix<ColorChannels>> mBase;
			DataMode mMode;
		};
	}
}
