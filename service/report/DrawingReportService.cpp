#include "DrawingReportService.h"
#include "extraction/BmpDrawer.hpp"

namespace am::service
{
    void DrawingReportService::generateReport(const std::string &base_img, const std::string &cmp_img, const std::string &out_diff_img, const analyze::algorithm::DescObjects &objects)
    {
        am::extraction::BmpDrawer drawer(base_img);
        for (auto &obj : objects)
        {
            drawer.drawRectangle(obj.getLeft(), obj.getMinHeight(), obj.getRight(), obj.getMaxHeight());
        }
        drawer.save(out_diff_img);
    }
}
