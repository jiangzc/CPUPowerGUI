#include "DisplayCorePage.h"
#include "CPUInfo.h"

QString processPolicyValue(const QString &name, const QString &value)
{
    QString res = value;
    if (name.endsWith("_freq"))
        res = QString("%0 MHz").arg(res.toInt() / 1000);
    else if(name == "energy_performance_available_preferences" || name == "scaling_available_governors")
    {
        res.replace(" ", "\n\t");
        res.insert(0, "\n\t");
    }
    return res;
}

DisplayCorePage::DisplayCorePage(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color:rgba(255,255,255,255)");
    CPUInfo cpuInfo;
    infoList = new QPlainTextEdit(this);
    QString text;
    text += QString("core id: %0\n").arg(cpuInfo.cores[0].core_id());
    text += QString("isEnabled: %0\n").arg(cpuInfo.cores[0].isEnabled());
    for (auto &item : cpuInfo.cores[0].policies)
    {
        QString value = processPolicyValue(item.name, item.value);
        text += QString("%0:\t%1\n").arg(item.name, value);
    }
    infoList->setPlainText(text);
    infoList->setTabStopWidth(40);
    QFont font = infoList->font();
    font.setPixelSize(18);
    infoList->setFont(font);
    infoList->move(50, 50);
    infoList->setReadOnly(true);
    infoList->resize(500,300);

    infoList->setBackgroundRole(QPalette::NoRole);
}

