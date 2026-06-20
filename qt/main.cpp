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

    spdlog::info("Aquamarine Qt App Starting...");

    // Create a rounded macOS-style icon
    QPixmap basePix(":/assets/app_icon.jpg");
    QIcon appIcon;
    if (!basePix.isNull()) {
        int side = std::min(basePix.width(), basePix.height());
        QPixmap square = basePix.copy((basePix.width() - side) / 2, (basePix.height() - side) / 2, side, side);
        
        QPixmap rounded(side, side);
        rounded.fill(Qt::transparent);
        QPainter painter(&rounded);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(0, 0, side, side, side * 0.22, side * 0.22); // macOS Squircle ratio
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, square);
        appIcon = QIcon(rounded);
        a.setWindowIcon(appIcon);
    }

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
