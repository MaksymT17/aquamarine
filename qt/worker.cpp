#include "worker.h"
#include <exception>
#include "extraction/MultipleExtractor.h"
#include "extraction/ExtractorFactory.h"
#include "extraction/BmpExtractor.h"
#ifndef WIN32
#include "extraction/JpgExtractor.h"
#endif
#include "analyze/algorithm/ObjectDetector.h"
#include "common/Context.hpp"

CompareWorker::CompareWorker(const QString& baseImg, const QString& cmpImg, const Configuration& config, QObject* parent)
    : QThread(parent)
    , m_baseImg(baseImg)
    , m_cmpImg(cmpImg)
    , m_config(config)
{
}

CompareWorker::~CompareWorker()
{
}

void CompareWorker::run()
{
    try {
        const size_t opt_threads = am::common::getOptimalThreadsCount(m_config.ThreadsMultiplier);
        
        auto factory = std::make_unique<am::extraction::ExtractorFactory>();
        factory->registerExtractor("bmp", am::extraction::BmpExtractor::readFile);
#ifndef WIN32
        factory->registerExtractor("jpg", am::extraction::JpgExtractor::readFile);
        factory->registerExtractor("jpeg", am::extraction::JpgExtractor::readFile);
        factory->registerExtractor("jpe", am::extraction::JpgExtractor::readFile);
#endif
        auto extractor = std::make_unique<am::extraction::MultipleExtractor>(std::move(factory));
        
        auto detector = std::make_unique<am::analyze::algorithm::ObjectDetector>(opt_threads, m_config);
        
        am::AmApi api(m_config, std::move(extractor), std::move(detector));
        am::analyze::algorithm::DescObjects result = api.compare(m_baseImg.toStdString(), m_cmpImg.toStdString());
        emit finishedSuccess(result);
    } catch (const std::exception& e) {
        emit finishedError(QString::fromStdString(e.what()));
    } catch (...) {
        emit finishedError("Unknown error occurred during comparison.");
    }
}
