#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "CPUInfo.h"


CPUInfo &CPUInfo::instance()
{
    static CPUInfo cpuinfo;
    return cpuinfo;
}

void CPUInfo::dump(QString filepath)
{
    QJsonDocument doc;
    QJsonArray array;
    for (auto &item : cores)
    {
        array.append(item.dumpSettings());
    }
    doc.setArray(array);
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}

void CPUInfo::load(QString filepath)
{
    QJsonDocument doc;
    QJsonObject obj;
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly))
    {
        doc = QJsonDocument::fromJson(file.readAll());
        file.close();
    }
    for (auto item : doc.array())
    {
        obj = item.toObject();
        int core_id = obj["id"].toInt();
        if (0 <= core_id && core_id < cores.count())
        {
            cores[core_id].loadSettings(obj);
        }
    }
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
