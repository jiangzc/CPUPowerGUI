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

    bool setEnabled(bool enable);
    QVector<CPUPolicy> policies;
    CPUPolicy findPolicyByName(QString name);
    bool setPolicy(QString name, QVariant value);
    bool update();
    short core_id() { return m_core_id; }
    bool isEnabled() { return isOnline; }

private:
    QDir m_coreDir;
    short m_core_id;
    bool isOnline = true;

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
