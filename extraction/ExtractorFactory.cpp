#include "ExtractorFactory.h"
#include "common/exceptions/WrongFormatException.hpp"

namespace am::extraction {
    void ExtractorFactory::registerExtractor(const std::string& ext, ExtractorFunc func) {
        m_extractors[ext] = std::move(func);
    }

    ExtractorFactory::ExtractorFunc ExtractorFactory::getExtractor(const std::string& ext) const {
        auto it = m_extractors.find(ext);
        if (it != m_extractors.end()) {
            return it->second;
        }
        std::string errorMsg("WrongFormatException: No extractor registered for extension: ");
        errorMsg.append(ext);
        throw am::common::exceptions::WrongFormatException(errorMsg);
    }
}
