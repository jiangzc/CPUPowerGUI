#ifndef CPUCORE_H
#define CPUCORE_H

#include <QString>
#include <QDir>
#include <QVector>

class CPUPolicy;

class CPUCore
{

public:
    CPUCore(const QDir &cpuDir);
    short core_id();
    bool isEnabled();
    bool setEnabled(bool enable);

    QVector<CPUPolicy> policies;
    CPUPolicy findPolicyByName(QString name);
    bool setPolicy(QString name, QVariant value);

    bool update();


private:
    QDir m_coreDir;
    short m_core_id;
    bool isOnline = true;

};

#endif // CPUCORE_H
