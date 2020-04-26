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


    qDebug() << "aaa";

}

DisplayUI::~DisplayUI()
{

}
