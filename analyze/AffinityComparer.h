#pragma once
#include "IComparer.h"
#include "common/types/Color24bDiff.hpp"
#include "common/types/Matrix.hpp"

namespace am
{
	namespace analyze
	{
		enum DataMode
		{
			KEEP_BASE_FRAME = 0,     // keep base frame for multiple comparisons
			SWITCH_TO_COMPARED = 1   // after every comparison base frame will replace to recently compared
		};

		class AffinityComparer : public IComparer
		{
		public:
			AffinityComparer(std::shared_ptr<common::types::Matrix<common::types::Color24b>> base);

			virtual ~AffinityComparer() = default;

			virtual std::shared_ptr<common::types::Matrix<common::types::Color24bDiff>>
				compare(std::shared_ptr<common::types::Matrix<common::types::Color24b>> newSource) override;

			static std::shared_ptr<common::types::Matrix<common::types::Color24bDiff>> compare(std::shared_ptr<common::types::Matrix<common::types::Color24b>> first, std::shared_ptr<common::types::Matrix<common::types::Color24b>> second);

			bool isRequestSizeValid(std::shared_ptr<common::types::Matrix<common::types::Color24b>> newSource) const;

		private:
			std::shared_ptr<common::types::Matrix<common::types::Color24b>> mBase;
			DataMode mMode;
		};
	}
}
