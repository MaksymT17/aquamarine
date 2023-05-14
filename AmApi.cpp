#include "AmApi.h"
#include <vector>
#include "extraction/BmpDrawer.hpp"
#include "analyze/algorithm/ImagePair.h"
#include <sstream>

#include <jpeglib.h>
namespace am
{
    using namespace common::types;
    using namespace analyze;


    AmApi::AmApi(const char *conf_path, bool is_bmp):
        loggerPtr(std::make_shared<am::common::Logger>("log.log")),
        extractor(loggerPtr, is_bmp),
        is_bmp_files(is_bmp)
    {
        am::configuration::ConfigurationReader reader;
        configuration = reader.getConfigurationFromFile(conf_path);
        const size_t opt_threads = am::common::getOptimalThreadsCount(configuration->ThreadsMultiplier);
        detector = std::make_unique<algorithm::ObjectDetector>(opt_threads, configuration, loggerPtr) ;
    }

    algorithm::DescObjects AmApi::compare(const std::string &base_img, const std::string &cmp_img)
    {
        std::vector<Matrix<Color24b>> data = extractor.readFiles({base_img, cmp_img});

        algorithm::ImagePair pair(data[0], data[1]);
        return detector->getObjectsRects(pair);
    }

    void AmApi::compare_and_save_diff_img(const std::string &base_img, const std::string &cmp_img, std::string && out_diff_img)
    {
        const algorithm::DescObjects objects = compare(base_img, cmp_img);
        am::extraction::BmpDrawer drawer(base_img);
        for (auto& obj : objects)
        {
            drawer.drawRectangle(obj.getLeft(), obj.getMinHeight(), obj.getRight(),
                                obj.getMaxHeight());
        }

        if(dbcPtr)
        {
            std::stringstream ss;
	        ss << "'" << am::common::get_datetime() << "', '" << base_img << "', '" << cmp_img << "', " << objects.size();
            dbcPtr->add_results_table(); // if already exists - it produces a warning
	        dbcPtr->insert_records2results({ss.str()});
        }
        drawer.save(out_diff_img);
    }

    void AmApi::enable_database_reports(const char *db_name)
    {
        dbcPtr.reset(new database::DataBaseCommunicator(db_name));
    }

    std::shared_ptr<configuration::Configuration> AmApi::getConfiguration()
    {
        return configuration;
    }

    void AmApi::setConfiguration(std::shared_ptr<configuration::Configuration> newConf)
    {
        configuration = newConf;
        const size_t opt_threads = am::common::getOptimalThreadsCount(configuration->ThreadsMultiplier);
        detector = std::make_unique<algorithm::ObjectDetector>(opt_threads, configuration, loggerPtr) ;
    }
}