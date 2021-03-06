#include <QDebug>
#include <QVariant>
#include <QJsonObject>
#include "CPUCore.h"
#include "CPUPolicy.h"

CPUCore::CPUCore(const QDir &cpuDir)
{
    m_coreDir = cpuDir;
    if (cpuDir.exists())
    {
        // get core id
        int id_len = 0;
        QString path = cpuDir.absolutePath();
        if (path[path.count() - 1].isNumber())
            id_len++;
        if (path[path.count() - 2].isNumber())
            id_len++;
        m_core_id = path.mid(path.count() - id_len, path.count() - 1).toShort();
        // load policies
        update();
    }
    else
    {
        qDebug() << "CPU Dir is not exist";
    }
}

bool CPUCore::setEnabled(bool enable)
{
    bool ret = false;
    QFile file(m_coreDir.absoluteFilePath("online"));
    if (file.exists() && file.open(QIODevice::WriteOnly))
    {
        if (file.write(enable ? "1" : "0") != -1)
        {
            ret = true;
            isOnline = enable;
        }
        file.close();
    }
    return ret;
}


bool CPUCore::setPolicy(QString name, QVariant value)
{
    bool ret = false;
    auto &policy = policies[name];

    if (policy.isWriteable && policy.name == name)
    {
        policy.value = value.toString();
        QFile file(m_coreDir.absoluteFilePath("cpufreq/" + name));
        if (file.exists() && file.open(QIODevice::WriteOnly))
        {
            if (file.write(value.toString().toLatin1()) != -1)
                ret = true;
            file.close();
        }
    }

    return ret;
}


QJsonObject CPUCore::dumpSettings()
{
    QJsonObject writablePolicies;
    for (auto &item : policies)
    {
        if (item.isWriteable)
        {
            writablePolicies[item.name] = item.value;
        }
    }
    QJsonObject obj;
    obj["id"] = core_id();
    obj["isEnabled"] = isEnabled();
    obj["policies"] = writablePolicies;
    return obj;
}

void CPUCore::loadSettings(const QJsonObject &obj)
{
    if (obj["id"] == core_id())
    {
        setEnabled(obj["isEnabled"].toBool(true));
        QJsonObject writablePolicies = obj["policies"].toObject();
        for (auto item = writablePolicies.constBegin(); item != writablePolicies.constEnd(); item++)
        {
            if (policies[item.key()].value != item.value().toString())
                setPolicy(item.key(), item.value().toString());
        }
    }
}


bool CPUCore::update()
{
    bool ret = true;
    QFile file;

    // is online ?
    file.setFileName(m_coreDir.absoluteFilePath("online"));
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        isOnline = file.readAll().trimmed().toInt();
        file.close();
    }
    // get policies
    policies.clear();
    QDir policyDir(m_coreDir.absoluteFilePath("cpufreq/"));
    auto infoList = policyDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    for (auto &info : infoList)
    {
        CPUPolicy policy;
        policy.name = info.fileName();
        file.setFileName(policyDir.absoluteFilePath(policy.name));
        if (file.open(QIODevice::ReadOnly))
        {
            policy.value = file.readAll().trimmed();
            file.close();
        }
        policy.isReadable = info.permission(QFileDevice::ReadOwner);
        policy.isWriteable = info.permission(QFileDevice::WriteOwner);
        policies.insert(policy.name, policy);
    }

    return ret;
}

bool CPUCore::checkPredefinedGoverns(PREDEFINED_GOVERNS gov)
{
    bool ret = isEnabled();
    const int cpuinfo_max_freq = policies[KnownCPUPolicy::cpuinfo_max_freq].value.toInt();
    const int scaling_max_freq = policies[KnownCPUPolicy::scaling_max_freq].value.toInt();

    if (gov == PREDEFINED_GOVERNS::Performance)
    {
        ret = ret  && (policies[KnownCPUPolicy::scaling_governor].value == "performance") &&
               (cpuinfo_max_freq == scaling_max_freq);
    }
    if (gov == PREDEFINED_GOVERNS::Fast)
    {
        ret = ret && (scaling_max_freq >= cpuinfo_max_freq * 2/3) ;
    }
    if (gov == PREDEFINED_GOVERNS::Normal)
    {
        ret = ret && (scaling_max_freq >= cpuinfo_max_freq * 1/2) && (scaling_max_freq < cpuinfo_max_freq * 2/3);
    }
    if (gov == PREDEFINED_GOVERNS::Slow)
    {
        ret = ret && (scaling_max_freq >= cpuinfo_max_freq * 1/3) && (scaling_max_freq < cpuinfo_max_freq * 1/2);
    }
    if (gov == PREDEFINED_GOVERNS::Powersave)
    {
        ret = ret && (policies[KnownCPUPolicy::scaling_governor].value == "powersave") &&
              (scaling_max_freq < cpuinfo_max_freq * 1/3);
    }

    return ret;
}

void CPUCore::setPredefinedGoverns(PREDEFINED_GOVERNS gov)
{
    const int cpuinfo_max_freq = policies[KnownCPUPolicy::cpuinfo_max_freq].value.toInt();
    const int cpuinfo_min_freq = policies[KnownCPUPolicy::cpuinfo_min_freq].value.toInt();
    setEnabled(true);
    if (gov == PREDEFINED_GOVERNS::Performance)
    {
        setPolicy(KnownCPUPolicy::scaling_governor, "performance");
        setPolicy(KnownCPUPolicy::scaling_min_freq, cpuinfo_min_freq);
        setPolicy(KnownCPUPolicy::scaling_max_freq, cpuinfo_max_freq);
    }
    if (gov == PREDEFINED_GOVERNS::Fast)
    {
        setPolicy(KnownCPUPolicy::scaling_governor, "performance");
        setPolicy(KnownCPUPolicy::scaling_min_freq, cpuinfo_min_freq);
        setPolicy(KnownCPUPolicy::scaling_max_freq, cpuinfo_max_freq * 2/3);
    }
    if (gov == PREDEFINED_GOVERNS::Normal)
    {
        setPolicy(KnownCPUPolicy::scaling_min_freq, cpuinfo_min_freq);
        setPolicy(KnownCPUPolicy::scaling_max_freq, cpuinfo_max_freq * 1/2);
    }
    if (gov == PREDEFINED_GOVERNS::Slow)
    {
        setPolicy(KnownCPUPolicy::scaling_min_freq, cpuinfo_min_freq);
        setPolicy(KnownCPUPolicy::scaling_max_freq, cpuinfo_max_freq * 1/3);
    }
    if (gov == PREDEFINED_GOVERNS::Powersave)
    {
        setPolicy(KnownCPUPolicy::scaling_governor, "powersave");
        setPolicy(KnownCPUPolicy::scaling_min_freq, cpuinfo_min_freq);
        setPolicy(KnownCPUPolicy::scaling_max_freq, cpuinfo_min_freq);
    }
}

short CPUCore::core_id() { return m_core_id; }

bool CPUCore::isEnabled() { return isOnline; }
