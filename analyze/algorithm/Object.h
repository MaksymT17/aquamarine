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

			struct Column {
				size_t left;
				size_t right;
			};

			// simple representatin of found object area on image, ismplified to rectangle bounds
			// base on diff pixels connected
			class Object {
			public:
				Object(std::vector<Pixel>& pixels);
				virtual ~Object() = default;
				bool isMergableToRight(Object& toCompare) const;
				bool mergeIfPossible(Object& toCompare);

				size_t getMinWidth() const;
				size_t getMaxWidth() const;
				size_t getMinHeight() const;
				size_t getMaxHeight() const;

			private:
				void mergeToMe(Object& toCompare);

				size_t mPixelsCount;
				size_t mMin_width;
				size_t mMin_height;
				size_t mMax_width;
				size_t mMax_height;
			};
		}
	}
}