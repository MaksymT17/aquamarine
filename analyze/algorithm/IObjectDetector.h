#pragma once

#include<memory>
#include"common/Types.hpp"

namespace recognition {
	namespace analyze {
		namespace algorithm {
			
			//define operator ==, to make std::find usage possible
			struct Position {
				size_t rowId;
				size_t colId;
			};

			class IObjectDetector
			{

			public:
				virtual ~IObjectDetector() = default;

				virtual std::vector<std::vector<Position>> getObjectsCount(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs) = 0;
			};
		}
	}
}