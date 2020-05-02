#include <QApplication>
#include <QColor>
#include <QFont>
#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QMessageBox>
#include "DisplayCorePage.h"
#include "CPUInfo.h"
#include "MainWindow.h"
#include "PolicyDisplayWidget.h"
#include "PolicyEditorWidget.h"
#include "SwitchHeader.h"
#include "MainPage.h"
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
//    SwitchHeader header(&w);
//    header.move(50, 20);

//    header.append("All");
//    for (int i = 0; i < cpuInfo.cores.count(); i++)
//    {
//        header.append("CPU " + QString::number(i));
//    }
//    header.adjustSize();
//    header.setCurrentIndex(0);
//    DisplayCorePage w2(cpuInfo.cores[1], &w);
//    w2.move(50,80);
    CPUInfo &cpuInfo = CPUInfo::instance();
    MainPage page(cpuInfo  ,&w);
    // page.show();
    w.resize(1200,700);
    w.show();


    return a.exec();
}
