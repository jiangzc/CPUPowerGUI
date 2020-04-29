#include <QDebug>
#include <QVariant>
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

short CPUCore::core_id() { return m_core_id; }

bool CPUCore::isEnabled() { return isOnline; }
