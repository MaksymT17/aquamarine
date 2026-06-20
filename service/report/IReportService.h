#pragma once
#include <string>
#include "analyze/algorithm/ObjectRectangle.h"

namespace am::service
{
    class IReportService
    {
    public:
        virtual ~IReportService() = default;
        virtual void generateReport(const std::string &base_img, const std::string &cmp_img, const std::string &out_diff_img, const analyze::algorithm::DescObjects &objects) = 0;
    };
}
