#ifndef CPUINFO_H
#define CPUINFO_H
#include <QObject>
#include <QString>
#include <QDir>
#include <QVector>
#include "CPUCore.h"
#include "CPUPolicy.h"

class CPUInfo
{
public:
    CPUInfo();
    QString modelName;
    QVector<CPUCore> cores;
private:
    QDir m_cpuDir;
};

#endif // CPUINFO_H
