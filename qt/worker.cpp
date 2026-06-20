#include "worker.h"
#include <exception>

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
        am::AmApi api(m_config);
        am::analyze::algorithm::DescObjects result = api.compare(m_baseImg.toStdString(), m_cmpImg.toStdString());
        emit finishedSuccess(result);
    } catch (const std::exception& e) {
        emit finishedError(QString::fromStdString(e.what()));
    } catch (...) {
        emit finishedError("Unknown error occurred during comparison.");
    }
}
