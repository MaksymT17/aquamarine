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
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // --- Top section: Image Selection ---
    QHBoxLayout* selectionLayout = new QHBoxLayout();
    
    QVBoxLayout* baseLayout = new QVBoxLayout();
    m_btnSelectBase = new QPushButton("Select Base Image");
    connect(m_btnSelectBase, &QPushButton::clicked, this, &MainWindow::selectBaseImage);
    m_lblBaseImage = new QLabel("No image selected");
    m_lblBaseImage->setAlignment(Qt::AlignCenter);
    m_lblBaseImage->setMinimumSize(320, 240);
    m_lblBaseImage->setStyleSheet("background-color: #1c1c1c; border: 1px solid #333;");
    baseLayout->addWidget(m_lblBaseImage);
    baseLayout->addWidget(m_btnSelectBase);
    
    QVBoxLayout* cmpLayout = new QVBoxLayout();
    m_btnSelectCompare = new QPushButton("Select Compare Image");
    connect(m_btnSelectCompare, &QPushButton::clicked, this, &MainWindow::selectCompareImage);
    m_lblCompareImage = new QLabel("No image selected");
    m_lblCompareImage->setAlignment(Qt::AlignCenter);
    m_lblCompareImage->setMinimumSize(320, 240);
    m_lblCompareImage->setStyleSheet("background-color: #1c1c1c; border: 1px solid #333;");
    cmpLayout->addWidget(m_lblCompareImage);
    cmpLayout->addWidget(m_btnSelectCompare);
    
    selectionLayout->addLayout(baseLayout);
    selectionLayout->addLayout(cmpLayout);
    
    mainLayout->addLayout(selectionLayout);
    
    // --- Middle section: Configuration & Run ---
    QHBoxLayout* middleLayout = new QHBoxLayout();
    
    QGroupBox* grpConfig = new QGroupBox("Configuration");
    QFormLayout* formLayout = new QFormLayout(grpConfig);
    
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
    m_btnRun = new QPushButton("Run Comparison");
    m_btnRun->setMinimumHeight(50);
    m_btnRun->setStyleSheet("font-weight: bold; background-color: #00d1ff; color: black; border-radius: 5px;");
    connect(m_btnRun, &QPushButton::clicked, this, &MainWindow::runComparison);
    
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 0); // Indeterminate
    m_progressBar->setVisible(false);
    
    m_lblStatus = new QLabel("Ready");
    
    runLayout->addStretch();
    runLayout->addWidget(m_btnRun);
    runLayout->addWidget(m_progressBar);
    runLayout->addWidget(m_lblStatus);
    runLayout->addStretch();
    
    middleLayout->addLayout(runLayout);
    middleLayout->setStretchFactor(grpConfig, 1);
    middleLayout->setStretchFactor(runLayout, 1);
    
    mainLayout->addLayout(middleLayout);
    
    // --- Bottom section: Result Image ---
    m_lblResultImage = new QLabel("Result will appear here");
    m_lblResultImage->setAlignment(Qt::AlignCenter);
    m_lblResultImage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_lblResultImage->setStyleSheet("background-color: #111; border: 2px dashed #444;");
    
    mainLayout->addWidget(m_lblResultImage, 1); // Give it stretch
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

    // Draw rectangles on a copy of the base image
    if (!m_baseQImage.isNull()) {
        QImage resultImg = m_baseQImage.copy();
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
        m_lblResultImage->setPixmap(pix.scaled(m_lblResultImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::onComparisonError(const QString& errorMsg)
{
    spdlog::error("Comparison failed: {}", errorMsg.toStdString());
    setUIEnabled(true);
    m_lblStatus->setText("Error occurred.");
    QMessageBox::critical(this, "Comparison Failed", errorMsg);
}
