#include <QDir>
#include <QFile>
#include <QDebug>
#include "CPUInfo.h"

#define KNOWN_POLICY(policy)   const char * const policy = #policy ;

namespace KnownCPUPolicy
{
    KNOWN_POLICY(cpuinfo_max_freq)
    KNOWN_POLICY(cpuinfo_min_freq)
    KNOWN_POLICY(scaling_available_governors)
    KNOWN_POLICY(scaling_governor)
    KNOWN_POLICY(scaling_cur_freq)
    KNOWN_POLICY(scaling_max_freq)
    KNOWN_POLICY(scaling_min_freq)
};

#undef KNOWN_POLICY

bool CPUPolicy::isEmpty()
{
    return name.isEmpty() && value.isEmpty();
}

CPUCore::CPUCore(const QDir &cpuDir)
{
    m_coreDir = cpuDir;
    if (cpuDir.exists())
    {
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

CPUPolicy CPUCore::findPolicyByName(QString name)
{
    for (auto &policy : policies)
    {
        if (name == policy.name)
            return policy;
    }
    return CPUPolicy();
}

bool CPUCore::setPolicy(QString name, QVariant value)
{
    bool ret = false;
    for (auto &policy : policies)
    {
        if (policy.isWriteable && policy.name == name)
        {
            policy.value = value.toString();
            QFile file(m_coreDir.absoluteFilePath("cpufreq/" + name));
            if (file.exists() && file.open(QIODevice::WriteOnly))
            {
                if (file.write(value.toString().toLatin1()) != -1)
                    ret = true;
                file.close();
                break;
            }
        }
    }
    return ret;
}


bool CPUCore::update()
{
    bool ret = true;

    // get core id
    QFile file;
    file.setFileName(m_coreDir.absoluteFilePath("topology/core_id"));
    ret = file.exists() && file.open(QIODevice::ReadOnly) && ret;
    if (ret)
    {
        m_core_id = file.readAll().toShort();
        file.close();
    }
    // is online ?
    file.setFileName(m_coreDir.absoluteFilePath("online"));
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        isOnline = file.readAll().toInt();
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
        policy.isReadable = info.isReadable();
        policy.isWriteable = info.isWritable();
        policies.append(policy);
    }

    return ret;
}

CPUInfo::CPUInfo() : m_cpuDir("/sys/devices/system/cpu")
{
    // get cores info
    auto dirList = m_cpuDir.entryList(QDir::Dirs);
    for (auto &fileName : dirList)
    {
        if (fileName.startsWith("cpu") && fileName.count() > 3 && fileName[3].isNumber())
        {
            if (fileName.count() == 4 || (fileName.count() == 5 && fileName[4].isNumber()))
                cores.append(CPUCore(QDir(m_cpuDir.absoluteFilePath(fileName))));
        }
    }
    // get model name
    QFile cpuinfo("/proc/cpuinfo");
    QString line;
    if (cpuinfo.open(QIODevice::ReadOnly))
    {
        while(true)
        {
            line = cpuinfo.readLine();
            if (!line.isEmpty())
            {
                if (line.startsWith("model name"))
                {
                    modelName = line.split(":")[1].trimmed();
                    break;
                }
            }
            else
                break;
        }

        cpuinfo.close();
    }
}
