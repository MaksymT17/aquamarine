#pragma once
#include <string>
#include <memory>

#include "extraction/BmpExtractor.h"
#include "configuration/ConfigurationReader.hpp"
#include "common/Logger.hpp"
#include "extraction/MultipleExtractor.h"
#ifndef WIN32
#include "database/DataBaseCommunicator.h"
#endif

namespace am
{
    class AmApi
    {
    public:
        AmApi(const Configuration& conf);
        analyze::algorithm::DescObjects compare(const std::string &base_img, const std::string &cmp_img);
        void compare_and_save_diff_img(const std::string &base_img, const std::string &cmp_img, std::string &&out_diff_img);
        void enable_database_reports(const char *db_name);
        const Configuration& getConfiguration();
        void setConfiguration(const Configuration& newConf);

    private:
        std::shared_ptr<am::common::Logger> loggerPtr;
        extraction::MultipleExtractor extractor;
        std::unique_ptr<analyze::algorithm::ObjectDetector> detector;

        std::string base_img_path;
        std::string cmp_img_path;
#ifndef WIN32
        std::unique_ptr<database::DataBaseCommunicator> dbcPtr;
#endif
        Configuration configuration;
    };
}