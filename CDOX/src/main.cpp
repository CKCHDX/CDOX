#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QPalette>
#include <QColor>

void setDarkNeonPalette(QApplication &app) {
    app.setStyle("Fusion");  // Use Fusion style for consistent appearance
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(1, 10, 22));
    darkPalette.setColor(QPalette::WindowText, QColor(0, 232, 255));
    darkPalette.setColor(QPalette::Base, QColor(0, 28, 46));
    darkPalette.setColor(QPalette::AlternateBase, QColor(0, 42, 71));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(0, 232, 255));
    darkPalette.setColor(QPalette::ToolTipText, QColor(1, 10, 22));
    darkPalette.setColor(QPalette::Text, QColor(0, 232, 255));
    darkPalette.setColor(QPalette::Button, QColor(0, 52, 81));
    darkPalette.setColor(QPalette::ButtonText, QColor(0, 232, 255));
    darkPalette.setColor(QPalette::BrightText, QColor(255, 0, 0));
    darkPalette.setColor(QPalette::Highlight, QColor(0, 183, 253));
    darkPalette.setColor(QPalette::HighlightedText, QColor(1, 10, 22));
    app.setPalette(darkPalette);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    setDarkNeonPalette(app);

    QFile f(":/styles/cdox.qss");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }

    MainWindow w;
    w.show();

    return app.exec();
}
