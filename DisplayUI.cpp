#include "DisplayUI.h"
#include "CPUInfo.h"
#include <QDebug>
#include <QDir>
#include <QMetaObject>
#include <QMetaProperty>

DisplayUI::DisplayUI(QWidget *parent)
    : QWidget(parent)
{

    CPUCore c1(QDir("/sys/devices/system/cpu/cpu0"));
    c1.update();
    const QMetaObject* metaObject = c1.metaObject();
    QStringList properties;
    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
    {
        properties << QString::fromLatin1(metaObject->property(i).name());
        auto name =  metaObject->property(i).name() ;
        qDebug() << name << " " <<  c1.property(name).toString();

    }
    c1.set_scaling_max_freq(2 * 1000 * 1000);
    c1.writePropertyToFile("scaling_max_freq");

}

DisplayUI::~DisplayUI()
{

}
