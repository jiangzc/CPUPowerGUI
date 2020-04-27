#include "DisplayUI.h"
#include "CPUInfo.h"
#include <QDebug>
#include <QDir>
#include <QMetaObject>
#include <QMetaProperty>

DisplayUI::DisplayUI(QWidget *parent)
    : QWidget(parent)
{

    CPUInfo cpuInfo;

//    qDebug() << cpuInfo.cores[0].setEnabled(true);
//    qDebug() << cpuInfo.cores[1].setEnabled(true);
//    qDebug() << cpuInfo.cores[2].setEnabled(true);
//    qDebug() << cpuInfo.cores[3].setEnabled(true);

    qDebug() <<  cpuInfo.cores[0].setPolicy(KnownCPUPolicy::cpuinfo_max_freq, 2000*1000);
    qDebug() <<  cpuInfo.cores[1].setPolicy(KnownCPUPolicy::cpuinfo_max_freq, 2000*1000);
    qDebug() <<  cpuInfo.cores[2].setPolicy(KnownCPUPolicy::cpuinfo_max_freq, 2000*1000);
    qDebug() <<  cpuInfo.cores[3].setPolicy(KnownCPUPolicy::cpuinfo_max_freq, 2000*1000);



}

DisplayUI::~DisplayUI()
{

}
