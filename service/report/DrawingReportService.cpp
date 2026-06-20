#include "DrawingReportService.h"
#include "extraction/ImageDrawer.hpp"

namespace am::service
{
    void DrawingReportService::generateReport(const std::string &base_img, const std::string &cmp_img, const std::string &out_diff_img, const analyze::algorithm::DescObjects &objects)
    {
        // The out_diff_img passed in usually has no extension or .bmp from old code.
        // We'll just append .png if it doesn't have it, or just let stb_image_write append it
        // Wait, DrawingReportService is given an exact path. I'll just save to that path. 
        // If it was .bmp before, saving a PNG as .bmp is slightly bad, but ImageDrawer explicitly writes PNG.
        // Let's modify out_diff_img to end in .png
        std::string finalPath = out_diff_img;
        if (finalPath.length() >= 4 && finalPath.substr(finalPath.length() - 4) == ".bmp") {
            finalPath = finalPath.substr(0, finalPath.length() - 4) + ".png";
        } else if (finalPath.find(".png") == std::string::npos) {
            finalPath += ".png";
        }

        am::extraction::ImageDrawer drawer(base_img);
        for (auto &obj : objects)
        {
            drawer.drawRectangle(obj.getLeft(), obj.getMinHeight(), obj.getRight(), obj.getMaxHeight());
        }
        drawer.save(finalPath);
    }
}
