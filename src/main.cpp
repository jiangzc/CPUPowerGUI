#include <QApplication>
#include <QFont>
#include <QFile>
#include <QScreen>
#include <QSize>
#include <QWindow>
#include <QMessageBox>
#include "MainWindow.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setSetuidAllowed(true);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
//    if (geteuid() != 0)
//    {
//        QMessageBox::critical(nullptr, "Error", "run it as root user");
//        abort();
//    }

    QFile qssFile(":/res/src/scrollbar.qss");
    if (qssFile.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(qssFile.readAll());
    }

    QFont font = a.font();
    font.setPixelSize(16);
    a.setFont(font);

    MainWindow w;
    w.adjustSize();
    QSize showPos = (a.primaryScreen()->availableSize() - w.size()) / 2;
    w.move(showPos.width(), showPos.height());
    w.show();
    w.resize(1200,650);

    return a.exec();
}
