#pragma once
#include "IReportService.h"
#include <memory>

#ifndef WIN32
namespace am::database { class DataBaseCommunicator; }
#endif

namespace am::service
{
    class DatabaseReportService : public IReportService
    {
    public:
        explicit DatabaseReportService(const std::string& db_name);
        ~DatabaseReportService() override;

        void generateReport(const std::string &base_img, const std::string &cmp_img, const std::string &out_diff_img, const analyze::algorithm::DescObjects &objects) override;
    
    private:
#ifndef WIN32
        std::unique_ptr<am::database::DataBaseCommunicator> dbcPtr;
#endif
    };
}
