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

    qDebug() << cpuInfo.cores[0].setEnabled(true);
    qDebug() << cpuInfo.cores[1].setEnabled(true);
    qDebug() << cpuInfo.cores[2].setEnabled(true);
    qDebug() << cpuInfo.cores[5].setEnabled(true);

    qDebug() << cpuInfo.cores[0].isEnabled();
    qDebug() << cpuInfo.cores[1].isEnabled();
    qDebug() << cpuInfo.cores[2].isEnabled();
    qDebug() << cpuInfo.cores[5].isEnabled();

}

DisplayUI::~DisplayUI()
{

}
