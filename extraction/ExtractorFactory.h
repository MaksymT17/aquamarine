#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"

namespace am::extraction {
    class ExtractorFactory {
    public:
        using ExtractorFunc = std::function<common::types::Matrix<common::types::Color24b>(const std::string&)>;
        
        ExtractorFactory() = default;
        ~ExtractorFactory() = default;
        
        void registerExtractor(const std::string& ext, ExtractorFunc func);
        ExtractorFunc getExtractor(const std::string& ext) const;
        
    private:
        std::unordered_map<std::string, ExtractorFunc> m_extractors;
    };
}
