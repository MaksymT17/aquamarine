#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <QStandardPaths>
#include <QDir>
#include <QPainter>
#include <QPainterPath>
#include <QIcon>
#include "mainwindow.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // --- Initialize spdlog ---
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(logDir); 
    std::string logPath = (logDir + "/aquamarine_qt.log").toStdString();

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); 
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath, false); 
    auto logger = std::make_shared<spdlog::logger>("am_logger", spdlog::sinks_init_list{console_sink, file_sink});
    
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    spdlog::set_level(spdlog::level::info);

    // No custom icon


    // Set a high-tech dark theme globally
    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(0, 0, 0));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(0, 0, 0));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(0, 209, 255));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    a.setPalette(darkPalette);

    MainWindow w;
    w.show();
    return a.exec();
}
