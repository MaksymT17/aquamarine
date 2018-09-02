#pragma once
#include<vector>
#include<cstdlib>
namespace am {
	namespace analyze {
		namespace algorithm {
			// position of Pixel on image 
			struct Pixel {
				size_t rowId;
				size_t colId;
			};

			// simple representatin of found object area on image, ismplified to rectangle bounds
			// base on diff pixels connected
			class Object {
			public:
				Object(std::vector<Pixel>& pixels);
				virtual ~Object() = default;
				bool isMergableToRight(Object& toCompare) const;
				void mergeToMe(Object& toCompare);
				bool mergeIfPossible(Object& toCompare);

				size_t getMinWidth() const;
				size_t getMaxWidth() const;
				size_t getMinHeight() const;
				size_t getMaxHeight() const;

			
				size_t min_width;
				size_t min_height;
				size_t max_width;
				size_t max_height;
			};
		}
	}
}