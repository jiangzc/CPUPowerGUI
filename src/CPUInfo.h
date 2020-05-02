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
    static CPUInfo& instance();
    QString modelName;
    QVector<CPUCore> cores;
private:
    QDir m_cpuDir;
    CPUInfo();
};


#endif // CPUINFO_H
