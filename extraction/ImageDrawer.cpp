#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../thirdparty/stb/stb_image.h"
#include "../thirdparty/stb/stb_image_write.h"
#include "ImageDrawer.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <algorithm>

namespace am::extraction
{
    ImageDrawer::ImageDrawer(const std::string &file)
        : mWidth(0), mHeight(0), mChannels(0), mData(nullptr)
    {
        mData = stbi_load(file.c_str(), &mWidth, &mHeight, &mChannels, 0);
        if (!mData) {
            spdlog::error("ImageDrawer failed to load: {}", file);
            throw std::runtime_error("Failed to load image for drawing.");
        }
    }

    ImageDrawer::~ImageDrawer()
    {
        if (mData) {
            stbi_image_free(mData);
        }
    }

    void ImageDrawer::drawRectangle(int minW, int minH, int maxW, int maxH)
    {
        if (!mData || mChannels < 3) return;

        auto setPixel = [&](int x, int y) {
            if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
                int index = (y * mWidth + x) * mChannels;
                mData[index] = 255;     // R
                mData[index + 1] = 0;   // G
                mData[index + 2] = 0;   // B
                if (mChannels == 4) {
                    mData[index + 3] = 255; // A
                }
            }
        };

        // Draw 2px thick line for visibility
        for (int thick = 0; thick < 2; ++thick) {
            // Horizontal lines
            for (int x = minW; x <= maxW; ++x) {
                setPixel(x, minH + thick);
                setPixel(x, maxH - thick);
            }

            // Vertical lines
            for (int y = minH; y <= maxH; ++y) {
                setPixel(minW + thick, y);
                setPixel(maxW - thick, y);
            }
        }
    }

    void ImageDrawer::save(const std::string &file)
    {
        if (!mData) return;
        
        // Write as PNG
        if (stbi_write_png(file.c_str(), mWidth, mHeight, mChannels, mData, mWidth * mChannels) == 0) {
             spdlog::error("ImageDrawer failed to save: {}", file);
        }
    }
}
