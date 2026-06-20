#pragma once
#include "IReportService.h"

namespace am::service
{
    class DrawingReportService : public IReportService
    {
    public:
        DrawingReportService() = default;
        ~DrawingReportService() override = default;

        void generateReport(const std::string &base_img, const std::string &cmp_img, const std::string &out_diff_img, const analyze::algorithm::DescObjects &objects) override;
    };
}
