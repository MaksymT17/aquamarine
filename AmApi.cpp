#include "AmApi.h"
#include <vector>
#include "extraction/BmpDrawer.hpp"
#include "analyze/algorithm/ImagePair.h"
#include <sstream>

#ifndef WIN32
#include <jpeglib.h>
#endif
namespace am
{
    using namespace common::types;
    using namespace analyze;

    AmApi::AmApi(const Configuration &conf) : loggerPtr(std::make_shared<am::common::Logger>("log.log")),
                                              extractor(loggerPtr)
    {
        configuration::ConfigurationReader reader;
        setConfiguration(conf);
    }

    algorithm::DescObjects AmApi::compare(const std::string &base_img, const std::string &cmp_img)
    {
        std::vector<Matrix<Color24b>> data = extractor.readFiles({base_img, cmp_img});
        algorithm::ImagePair pair(data[0], data[1]);
        auto res = detector->getObjectsRects(pair);
        return res;
    }

    void AmApi::compare_and_save_diff_img(const std::string &base_img, const std::string &cmp_img, std::string &&out_diff_img)
    {
        const algorithm::DescObjects objects = compare(base_img, cmp_img);
        am::extraction::BmpDrawer drawer(base_img);
        for (auto &obj : objects)
        {
            drawer.drawRectangle(obj.getLeft(), obj.getMinHeight(), obj.getRight(),
                                 obj.getMaxHeight());
        }

#ifndef WIN32
        if (dbcPtr)
        {
            std::stringstream ss;
            ss << "'" << am::common::get_datetime() << "', '" << base_img << "', '" << cmp_img << "', " << objects.size();
            dbcPtr->add_results_table(); // if already exists - it produces a warning
            dbcPtr->insert_records2results({ss.str()});
        }
#endif
        drawer.save(out_diff_img);
    }

    void AmApi::enable_database_reports(const char *db_name)
    {
#ifndef WIN32
        dbcPtr.reset(new database::DataBaseCommunicator(db_name));
#endif
    }

    const Configuration &AmApi::getConfiguration()
    {
        return configuration;
    }

    void AmApi::setConfiguration(const Configuration &newConf)
    {
        configuration = newConf;
        const size_t opt_threads = am::common::getOptimalThreadsCount(configuration.ThreadsMultiplier);
        detector = std::make_unique<algorithm::ObjectDetector>(opt_threads, configuration, loggerPtr);
    }
}