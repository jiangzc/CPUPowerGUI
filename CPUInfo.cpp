#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include "CPUInfo.h"

CPUInfo::CPUInfo()
{

}

CPUCore::CPUCore(const QDir &cpuDir)
{
    m_cpuDir = cpuDir;
    if (!cpuDir.exists())
        qDebug() << "CPU Dir is not exist";
}

unsigned int CPUCore::cpuinfo_max_freq() const
{
    return m_cpuinfo_max_freq;
}

unsigned int CPUCore::cpuinfo_min_freq() const
{
    return m_cpuinfo_min_freq;
}

unsigned int CPUCore::scaling_max_freq() const
{
    return m_scaling_max_freq;
}

unsigned int CPUCore::scaling_min_freq() const
{
    return m_scaling_min_freq;
}

unsigned int CPUCore::scaling_cur_freq() const
{
    return m_scaling_cur_freq;
}

QString CPUCore::scaling_available_governors() const
{
    return m_scaling_available_governors;
}

QString CPUCore::scaling_governor() const
{
    return m_scaling_governor;
}

QString CPUCore::scaling_driver() const
{
    return m_scaling_driver;
}

void CPUCore::set_scaling_max_freq(unsigned int scaling_max_freq)
{
    m_scaling_max_freq = scaling_max_freq;
}

void CPUCore::set_scaling_min_freq(unsigned int scaling_min_freq)
{
    m_scaling_min_freq = scaling_min_freq;
}

void CPUCore::set_scaling_governor(QString scaling_governor)
{
    m_scaling_governor = scaling_governor;
}

bool CPUCore::update()
{
    bool ret = true;

    // get core id
    QFile file;
    file.setFileName(m_cpuDir.absoluteFilePath("topology/core_id"));
    ret = file.exists() && file.open(QIODevice::ReadOnly) && ret;
    if (ret)
    {
        core_id = file.readAll().toShort();
        file.close();
    }
    // get properties value
    const QMetaObject* metaObject = this->metaObject();
    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
    {
        const char *key = metaObject->property(i).name();
        file.setFileName(m_cpuDir.absoluteFilePath("cpufreq/" + QString(key)));
        // don't use ret for dynamic property
        if (file.exists() && file.open(QIODevice::ReadOnly))
        {
            this->setProperty(key, file.readAll().trimmed());
            file.close();
        }
    }

    return ret;

}

bool CPUCore::writePropertyToFile(QString key)
{
    qDebug() << "aaa";
    bool ret = true;
    QFile file;
    file.setFileName(m_cpuDir.absoluteFilePath("cpufreq/" + key));
    // qDebug() << file.fileName();
    ret = file.exists() && file.open(QIODevice::WriteOnly) && ret;
    if (ret)
    {
        file.write(this->property(key.toLatin1().data()).toByteArray().trimmed());
        file.close();
    }
    return  ret;
}
