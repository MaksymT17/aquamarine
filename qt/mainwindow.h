#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QImage>

#include "AmApi.h"

class CompareWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectBaseImage();
    void selectCompareImage();
    void runComparison();
    void onComparisonSuccess(const am::analyze::algorithm::DescObjects& result);
    void onComparisonError(const QString& errorMsg);

private:
    void updateImageView(QLabel* label, const QString& path);
    void setupUI();
    Configuration getConfigurationFromUI() const;
    void setUIEnabled(bool enabled);

    // UI elements
    QLabel* m_lblBaseImage;
    QLabel* m_lblCompareImage;
    QLabel* m_lblResultImage;
    
    QPushButton* m_btnSelectBase;
    QPushButton* m_btnSelectCompare;
    QPushButton* m_btnRun;
    
    QSpinBox* m_spnAffinityThreshold;
    QSpinBox* m_spnMinPixels;
    QSpinBox* m_spnPixelStep;
    QDoubleSpinBox* m_spnTimeLimit;
    QSpinBox* m_spnIdleTimeout;
    QDoubleSpinBox* m_spnThreadsMultiplier;
    
    QProgressBar* m_progressBar;
    QLabel* m_lblStatus;

    // Data
    QString m_baseImagePath;
    QString m_compareImagePath;
    QImage m_baseQImage;

    CompareWorker* m_worker;
};
