#include <QApplication>
#include <QColor>
#include <QFont>
#include <QFile>
#include <QMessageBox>
#include "DisplayUI.h"
#include "DisplayCorePage.h"
#include "CPUInfo.h"
#include "MainWindow.h"
#include "PolicyDisplayWidget.h"
#include "PolicyEditorWidget.h"
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
    a.setPalette( QPalette(QColor(247, 247, 247) ));

    CPUInfo cpuInfo;
    MainWindow w;
//    DisplayCorePage w2(cpuInfo.cores[1], &w);
//    w.resize(1100,600);
//    w.show();
    PolicyEditorWidget w2(cpuInfo.cores[1], &w);
    w.show();
    return a.exec();
}
