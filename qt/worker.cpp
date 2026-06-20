#include "worker.h"
#include <exception>
#include "extraction/MultipleExtractor.h"
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
        auto extractor = std::make_unique<am::extraction::MultipleExtractor>();
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
