#include <QApplication>
#include <QColor>
#include <QFont>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include "DisplayUI.h"
#include "DisplayCorePage.h"
#include "CPUInfo.h"
#include "MainWindow.h"
#include "PolicyDisplayWidget.h"
#include "PolicyEditorWidget.h"
#include "SwitchHeader.h"
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

    CPUInfo cpuInfo;
    MainWindow w;
    SwitchHeader header(&w);
    header.setGeometry(20,10,300,40);
    header.append("aa");
    header.append("aa");
    header.append("aa");
    header.append("aa");
    header.append("aa");
    header.setCurrentIndex(0);
    DisplayCorePage w2(cpuInfo.cores[1], &w);
    w2.move(50,50);
    w.resize(1100,600);
    w.show();

    return a.exec();
}
