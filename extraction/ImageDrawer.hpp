#pragma once
#include <string>

namespace am::extraction
{
    class ImageDrawer
    {
    public:
        explicit ImageDrawer(const std::string &file);
        ~ImageDrawer();

        void drawRectangle(int minW, int minH, int maxW, int maxH);
        void save(const std::string &file);

    private:
        int mWidth;
        int mHeight;
        int mChannels;
        unsigned char* mData;
    };
} // namespace am::extraction
