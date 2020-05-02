#ifndef CPUCORE_H
#define CPUCORE_H

#include <QString>
#include <QDir>
#include <QMap>

class CPUPolicy;

enum PREDEFINED_GOVERNS {
    Performance,
    Fast,
    Normal,
    Slow,
    Powersave,
    Custom
};

class CPUCore
{

public:
    CPUCore() {} // fix error in Qt version 5.7.1
    CPUCore(const QDir &cpuDir);
    short core_id();
    bool isEnabled();
    bool setEnabled(bool enable);

    QMap<QString, CPUPolicy> policies;
    bool setPolicy(QString name, QVariant value);
    QJsonObject dumpSettings();
    void loadSettings(const QJsonObject &obj);
    bool update();
    bool checkPredefinedGoverns(PREDEFINED_GOVERNS gov);
    void setPredefinedGoverns(PREDEFINED_GOVERNS gov);

private:
    QDir m_coreDir;
    short m_core_id;
    bool isOnline = true;

};

#endif // CPUCORE_H
