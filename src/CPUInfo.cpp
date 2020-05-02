#include <QDir>
#include <QFile>
#include <QDebug>
#include "CPUInfo.h"


CPUInfo &CPUInfo::instance()
{
    static CPUInfo cpuinfo;
    return cpuinfo;
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
