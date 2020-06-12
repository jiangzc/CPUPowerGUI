#include <QApplication>
#include <QFont>
#include <QFile>
#include <QScreen>
#include <QSize>
#include <QDebug>
#include <QDir>
#include <QWindow>
#include <QMessageBox>
#include "MainWindow.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setSetuidAllowed(true);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    //  ensure to run it as root user
    if (geteuid() != 0 && QCoreApplication::applicationDirPath().startsWith("/opt"))
    {
        QMessageBox::critical(nullptr, "Error", "Run it as root user");
        abort();
    }

    QFile qssFile(":/res/src/ui.qss");
    if (qssFile.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(qssFile.readAll());
        qssFile.close();
    }

    QPalette palette = a.palette();
    palette.setColor(QPalette::ColorRole::Highlight, QColor("#1ABC9C"));
    palette.setColor(QPalette::ColorRole::WindowText, QColor("#000000"));
    palette.setColor(QPalette::ColorRole::Text, QColor("#000000"));
    palette.setColor(QPalette::ColorRole::Window, QColor("#FFFFFF"));
    palette.setColor(QPalette::ColorRole::Background, QColor("#FFFFFF"));
    palette.setColor(QPalette::ColorRole::Base, QColor("#FFFFFF"));
    palette.setColor(QPalette::ColorRole::Button, QColor("#FFFFFF"));
    a.setPalette(palette);

    QFont font = a.font();
    font.setPixelSize(17);
    a.setFont(font);

    MainWindow w;
    w.adjustSize();
    QSize showPos = (a.primaryScreen()->availableSize() - w.size()) / 2;
    w.move(showPos.width(), showPos.height());
    w.show();
    w.resize(1200,650);

    return a.exec();
}
