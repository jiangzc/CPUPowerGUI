#ifndef CPUINFO_H
#define CPUINFO_H
#include <QObject>
#include <QString>
#include <QDir>
#include <QVector>

struct CPUPolicy
{
public:
    QString name;
    QString value;
    bool isReadable = false;
    bool isWriteable = false;
    bool isEmpty();
};


class CPUCore
{

public:
    CPUCore(const QDir &cpuDir);
    short core_id;
    bool isOnline = true;
    QVector<CPUPolicy> policies;
    CPUPolicy findPolicyByName(QString name);
    bool setPolicy(QString name, QVariant value);
    bool update();


private:
    QDir m_coreDir;


};

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
