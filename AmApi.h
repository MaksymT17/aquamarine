#pragma once
#include <string>
#include <memory>

#include "extraction/IMultipleExtractor.h"
#include "analyze/algorithm/IObjectDetector.h"
#include "configuration/ConfigurationReader.hpp"

namespace am
{
    class AmApi
    {
    public:
        AmApi(const Configuration& conf, 
              std::unique_ptr<extraction::IMultipleExtractor> extractor, 
              std::unique_ptr<analyze::algorithm::IObjectDetector> detector);
        analyze::algorithm::DescObjects compare(const std::string &base_img, const std::string &cmp_img);
        const Configuration& getConfiguration();

    private:
        std::unique_ptr<extraction::IMultipleExtractor> extractor;
        std::unique_ptr<analyze::algorithm::IObjectDetector> detector;

        Configuration configuration;
    };
}