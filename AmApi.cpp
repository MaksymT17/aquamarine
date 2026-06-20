#include "AmApi.h"
#include <vector>
#include "analyze/algorithm/ImagePair.h"
namespace am
{
    using namespace common::types;
    using namespace analyze;

    AmApi::AmApi(const Configuration &conf, 
                 std::unique_ptr<extraction::IMultipleExtractor> ext, 
                 std::unique_ptr<analyze::algorithm::IObjectDetector> det)
        : extractor(std::move(ext)), detector(std::move(det)), configuration(conf)
    {
    }

    algorithm::DescObjects AmApi::compare(const std::string &base_img, const std::string &cmp_img)
    {
        std::vector<Matrix<Color24b>> data = extractor->readFiles({base_img, cmp_img});
        algorithm::ImagePair pair(data[0], data[1]);
        auto res = detector->getObjectsRects(pair);
        return res;
    }

    const Configuration &AmApi::getConfiguration()
    {
        return configuration;
    }

}