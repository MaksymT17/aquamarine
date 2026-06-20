#include "mainwindow.h"
#include "worker.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QFormLayout>
#include <spdlog/spdlog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_worker(nullptr)
{
    setupUI();
    setWindowTitle("Aquamarine Analysis");
    resize(1024, 768);
}

MainWindow::~MainWindow()
{
    if (m_worker && m_worker->isRunning()) {
        m_worker->wait();
    }
}

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Set a global stylesheet for the application
    this->setStyleSheet(
        "QWidget { font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif; font-size: 13px; color: #e0e0e0; }"
        "QGroupBox { font-weight: bold; border: 1px solid #333; border-radius: 6px; margin-top: 2ex; background-color: #1a1a1a; padding-top: 15px; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 5px; color: #00d1ff; left: 10px; font-size: 14px; font-weight: bold; }"
        "QPushButton { background-color: #2c2c2c; color: #fff; border: 1px solid #444; border-radius: 4px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #3a3a3a; border-color: #555; }"
        "QPushButton:pressed { background-color: #222; border-color: #333; }"
        "QPushButton:disabled { background-color: #111; color: #555; border-color: #222; }"
        "QSpinBox, QDoubleSpinBox { background-color: #111; color: #00d1ff; border: 1px solid #333; border-radius: 4px; padding: 4px 8px; font-family: 'Menlo', monospace; font-weight: bold; selection-background-color: #00d1ff; selection-color: black; }"
        "QSpinBox::up-button, QSpinBox::down-button, QDoubleSpinBox::up-button, QDoubleSpinBox::down-button { width: 0px; border: none; }"
        "QProgressBar { border: 1px solid #444; border-radius: 4px; background-color: #111; text-align: center; color: white; font-weight: bold; }"
        "QProgressBar::chunk { background-color: #00d1ff; border-radius: 3px; }"
        "QLabel#imagePlaceholder { background-color: #151515; border: 2px dashed #444; border-radius: 8px; color: #666; font-weight: bold; font-size: 14px; }"
        "QLabel#statusLabel { color: #00d1ff; font-weight: bold; font-size: 14px; }"
    );
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(20);
    
    // --- Top section: Image Selection ---
    QHBoxLayout* selectionLayout = new QHBoxLayout();
    selectionLayout->setSpacing(20);
    
    QVBoxLayout* baseLayout = new QVBoxLayout();
    m_lblBaseImage = new QLabel("SELECT BASE IMAGE");
    m_lblBaseImage->setObjectName("imagePlaceholder");
    m_lblBaseImage->setAlignment(Qt::AlignCenter);
    m_lblBaseImage->setMinimumSize(320, 240);
    m_btnSelectBase = new QPushButton("Browse Base Image");
    connect(m_btnSelectBase, &QPushButton::clicked, this, &MainWindow::selectBaseImage);
    baseLayout->addWidget(m_lblBaseImage);
    baseLayout->addWidget(m_btnSelectBase);
    
    QVBoxLayout* cmpLayout = new QVBoxLayout();
    m_lblCompareImage = new QLabel("SELECT COMPARE IMAGE");
    m_lblCompareImage->setObjectName("imagePlaceholder");
    m_lblCompareImage->setAlignment(Qt::AlignCenter);
    m_lblCompareImage->setMinimumSize(320, 240);
    m_btnSelectCompare = new QPushButton("Browse Compare Image");
    connect(m_btnSelectCompare, &QPushButton::clicked, this, &MainWindow::selectCompareImage);
    cmpLayout->addWidget(m_lblCompareImage);
    cmpLayout->addWidget(m_btnSelectCompare);
    
    selectionLayout->addLayout(baseLayout);
    selectionLayout->addLayout(cmpLayout);
    
    mainLayout->addLayout(selectionLayout, 1);
    
    // --- Middle section: Configuration & Run ---
    QHBoxLayout* middleLayout = new QHBoxLayout();
    middleLayout->setSpacing(20);
    
    QGroupBox* grpConfig = new QGroupBox("ANALYSIS CONFIGURATION");
    QFormLayout* formLayout = new QFormLayout(grpConfig);
    formLayout->setContentsMargins(20, 20, 20, 20);
    formLayout->setSpacing(12);
    
    m_spnAffinityThreshold = new QSpinBox();
    m_spnAffinityThreshold->setRange(0, 255);
    m_spnAffinityThreshold->setValue(75);
    
    m_spnMinPixels = new QSpinBox();
    m_spnMinPixels->setRange(1, 10000);
    m_spnMinPixels->setValue(10);
    
    m_spnPixelStep = new QSpinBox();
    m_spnPixelStep->setRange(1, 100);
    m_spnPixelStep->setValue(1);
    
    m_spnTimeLimit = new QDoubleSpinBox();
    m_spnTimeLimit->setRange(0.1, 1000.0);
    m_spnTimeLimit->setValue(50.0);
    
    m_spnIdleTimeout = new QSpinBox();
    m_spnIdleTimeout->setRange(1, 1000);
    m_spnIdleTimeout->setValue(5);
    
    m_spnThreadsMultiplier = new QDoubleSpinBox();
    m_spnThreadsMultiplier->setRange(0.1, 10.0);
    m_spnThreadsMultiplier->setValue(10.0);
    
    formLayout->addRow("Affinity Threshold:", m_spnAffinityThreshold);
    formLayout->addRow("Min Pixels For Object:", m_spnMinPixels);
    formLayout->addRow("Pixel Step:", m_spnPixelStep);
    formLayout->addRow("Calculation Time Limit (s):", m_spnTimeLimit);
    formLayout->addRow("Idle Timeout (s):", m_spnIdleTimeout);
    formLayout->addRow("Threads Multiplier:", m_spnThreadsMultiplier);
    
    middleLayout->addWidget(grpConfig);
    
    QVBoxLayout* runLayout = new QVBoxLayout();
    runLayout->setContentsMargins(10, 10, 10, 10);
    runLayout->setSpacing(15);
    
    m_btnRun = new QPushButton("RUN COMPARISON");
    m_btnRun->setMinimumHeight(60);
    m_btnRun->setStyleSheet(
        "QPushButton { font-size: 16px; letter-spacing: 1px; font-weight: bold; background-color: #00d1ff; color: #000; border: none; border-radius: 6px; }"
        "QPushButton:hover { background-color: #33daff; }"
        "QPushButton:pressed { background-color: #00aacc; }"
        "QPushButton:disabled { background-color: #113333; color: #005566; }"
    );
    connect(m_btnRun, &QPushButton::clicked, this, &MainWindow::runComparison);
    
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 0); // Indeterminate
    m_progressBar->setVisible(false);
    m_progressBar->setFixedHeight(6);
    
    m_lblStatus = new QLabel("SYSTEM READY");
    m_lblStatus->setObjectName("statusLabel");
    m_lblStatus->setAlignment(Qt::AlignCenter);
    
    runLayout->addStretch();
    runLayout->addWidget(m_btnRun);
    runLayout->addWidget(m_progressBar);
    runLayout->addWidget(m_lblStatus);
    runLayout->addStretch();
    
    middleLayout->addLayout(runLayout);
    middleLayout->setStretchFactor(grpConfig, 1);
    middleLayout->setStretchFactor(runLayout, 1);
    
    mainLayout->addLayout(middleLayout);
    
}

void MainWindow::selectBaseImage()
{
    QString path = QFileDialog::getOpenFileName(this, "Select Base Image", "", "Images (*.png *.xpm *.jpg *.bmp)");
    if (!path.isEmpty()) {
        m_baseImagePath = path;
        m_baseQImage.load(path);
        updateImageView(m_lblBaseImage, path);
    }
}

void MainWindow::selectCompareImage()
{
    QString path = QFileDialog::getOpenFileName(this, "Select Compare Image", "", "Images (*.png *.xpm *.jpg *.bmp)");
    if (!path.isEmpty()) {
        m_compareImagePath = path;
        m_compareQImage.load(path);
        updateImageView(m_lblCompareImage, path);
    }
}

void MainWindow::updateImageView(QLabel* label, const QString& path)
{
    QPixmap pix(path);
    if (!pix.isNull()) {
        label->setPixmap(pix.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        label->setText("Failed to load image");
    }
}

Configuration MainWindow::getConfigurationFromUI() const
{
    Configuration conf;
    conf.AffinityThreshold = m_spnAffinityThreshold->value();
    conf.MinPixelsForObject = m_spnMinPixels->value();
    conf.PixelStep = m_spnPixelStep->value();
    conf.CalculationTimeLimit = m_spnTimeLimit->value();
    conf.IdleTimeout = m_spnIdleTimeout->value();
    conf.ThreadsMultiplier = m_spnThreadsMultiplier->value();
    return conf;
}

void MainWindow::setUIEnabled(bool enabled)
{
    m_btnSelectBase->setEnabled(enabled);
    m_btnSelectCompare->setEnabled(enabled);
    m_btnRun->setEnabled(enabled);
    m_progressBar->setVisible(!enabled);
}

void MainWindow::runComparison()
{
    if (m_baseImagePath.isEmpty() || m_compareImagePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select both base and compare images.");
        return;
    }

    if (m_worker && m_worker->isRunning()) {
        return; // Already running
    }

    if (m_worker) {
        m_worker->deleteLater();
    }

    setUIEnabled(false);
    m_lblStatus->setText("Running comparison...");
    spdlog::info("Starting comparison between {} and {}", m_baseImagePath.toStdString(), m_compareImagePath.toStdString());

    Configuration conf = getConfigurationFromUI();
    m_worker = new CompareWorker(m_baseImagePath, m_compareImagePath, conf, this);
    
    connect(m_worker, &CompareWorker::finishedSuccess, this, &MainWindow::onComparisonSuccess);
    connect(m_worker, &CompareWorker::finishedError, this, &MainWindow::onComparisonError);
    
    m_worker->start();
}

void MainWindow::onComparisonSuccess(const am::analyze::algorithm::DescObjects& result)
{
    spdlog::info("Comparison successful. Objects found: {}", result.size());
    setUIEnabled(true);
    m_lblStatus->setText(QString("Done. Found %1 objects.").arg(result.size()));

    // Draw rectangles on a copy of the compare image
    if (!m_compareQImage.isNull()) {
        QImage resultImg = m_compareQImage.copy();
        QPainter painter(&resultImg);
        
        QPen pen(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);

        for (const auto& obj : result) {
            int x = obj.getLeft();
            int y = obj.getMinHeight();
            int w = obj.getRight() - x;
            int h = obj.getMaxHeight() - y;
            painter.drawRect(x, y, w, h);
        }

        QPixmap pix = QPixmap::fromImage(resultImg);
        m_lblCompareImage->setPixmap(pix.scaled(m_lblCompareImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::onComparisonError(const QString& errorMsg)
{
    spdlog::error("Comparison failed: {}", errorMsg.toStdString());
    setUIEnabled(true);
    m_lblStatus->setText("Error occurred.");
    QMessageBox::critical(this, "Comparison Failed", errorMsg);
}
