#include <string>
#include <memory>

#include "extraction/BmpExtractor.h"
#include "configuration/ConfigurationReader.hpp"
#include "common/Logger.hpp"
#include "extraction/MultipleBmpExtractor.h"
#include "database/DataBaseCommunicator.h"

namespace am
{
    class AmApi
    {
    public:
        AmApi(const char *conf_path);
        analyze::algorithm::DescObjects compare(const std::string &base_img, const std::string &cmp_img);
        void compare_and_save_diff_img(const std::string &base_img, const std::string &cmp_img, std::string &&out_diff_img);
        void enable_database_reports(const char *db_name);
        std::shared_ptr<configuration::Configuration> getConfiguration();
        void setConfiguration(std::shared_ptr<configuration::Configuration> newConf);

    private:
        std::shared_ptr<am::common::Logger> loggerPtr;
        extraction::MultipleBmpExtractor extractor;
        std::unique_ptr<analyze::algorithm::ObjectDetector> detector;

        std::string base_img_path;
        std::string cmp_img_path;
        std::unique_ptr<database::DataBaseCommunicator> dbcPtr;
        std::shared_ptr<configuration::Configuration> configuration;
    };
}