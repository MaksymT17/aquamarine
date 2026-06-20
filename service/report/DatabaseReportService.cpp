#include "DatabaseReportService.h"
#include <sstream>
#include "common/Timers.hpp"

#ifndef WIN32
#include "database/DataBaseCommunicator.h"
#endif

namespace am::service
{
    DatabaseReportService::DatabaseReportService(const std::string& db_name)
    {
#ifndef WIN32
        if (!db_name.empty()) {
            dbcPtr = std::make_unique<am::database::DataBaseCommunicator>(db_name.c_str());
        }
#endif
    }

    DatabaseReportService::~DatabaseReportService() = default;

    void DatabaseReportService::generateReport(const std::string &base_img, const std::string &cmp_img, const std::string &out_diff_img, const analyze::algorithm::DescObjects &objects)
    {
#ifndef WIN32
        if (dbcPtr)
        {
            std::string safe_base = base_img;
            std::string safe_cmp = cmp_img;
            auto sanitize = [](std::string& str) {
                size_t pos = 0;
                while ((pos = str.find("'", pos)) != std::string::npos) {
                    str.replace(pos, 1, "''");
                    pos += 2;
                }
            };
            sanitize(safe_base);
            sanitize(safe_cmp);

            std::stringstream ss;
            ss << "'" << am::common::get_datetime() << "', '" << safe_base << "', '" << safe_cmp << "', " << objects.size();
            dbcPtr->add_results_table(); // if already exists - it produces a warning
            dbcPtr->insert_records2results({ss.str()});
        }
#endif
    }
}
