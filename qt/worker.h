#pragma once

#include <QThread>
#include <QString>
#include "AmApi.h"

class CompareWorker : public QThread
{
    Q_OBJECT
public:
    CompareWorker(const QString& baseImg, const QString& cmpImg, const Configuration& config, QObject* parent = nullptr);
    ~CompareWorker();

protected:
    void run() override;

signals:
    void finishedSuccess(const am::analyze::algorithm::DescObjects& result);
    void finishedError(const QString& errorMsg);

private:
    QString m_baseImg;
    QString m_cmpImg;
    Configuration m_config;
};
