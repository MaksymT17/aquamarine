#pragma once
#include <string>
#include <memory>

#include "extraction/IMultipleExtractor.h"
#include "analyze/algorithm/IObjectDetector.h"
#include "configuration/ConfigurationReader.hpp"
#ifndef WIN32
#include "database/DataBaseCommunicator.h"
#endif

namespace am
{
    class AmApi
    {
    public:
        AmApi(const Configuration& conf, 
              std::unique_ptr<extraction::IMultipleExtractor> extractor, 
              std::unique_ptr<analyze::algorithm::IObjectDetector> detector);
        analyze::algorithm::DescObjects compare(const std::string &base_img, const std::string &cmp_img);
        void compare_and_save_diff_img(const std::string &base_img, const std::string &cmp_img, std::string &&out_diff_img);
        void enable_database_reports(const char *db_name);
        const Configuration& getConfiguration();

    private:
        std::unique_ptr<extraction::IMultipleExtractor> extractor;
        std::unique_ptr<analyze::algorithm::IObjectDetector> detector;

        std::string base_img_path;
        std::string cmp_img_path;
#ifndef WIN32
        std::unique_ptr<database::DataBaseCommunicator> dbcPtr;
#endif
        Configuration configuration;
    };
}