#include <QScrollArea>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include "DisplayCorePage.h"
#include "CPUInfo.h"

QString DisplayCorePage::processPolicyValue(const CPUPolicy &policy)
{
    QString res = policy.value;
    if (policy.name.endsWith("_freq"))
        res = QString("%0 MHz").arg(res.toInt() / 1000);
    else if(policy.name == "energy_performance_available_preferences" || policy.name == "scaling_available_governors")
    {
        res.replace(" ", "\n");
    }
    return res;
}

void DisplayCorePage::updatePolicyValue(const CPUPolicy &policy)
{
    for (const auto &item : infoList->children())
    {
        if (item->objectName() == policy.name)
        {
            QLabel *label = dynamic_cast<QLabel*>(item);
            if (label != nullptr)
                label->setText(processPolicyValue(policy));
        }
    }
}

QWidget* DisplayCorePage::getPolicyValueWidget(const CPUPolicy &policy)
{
    QLabel *label = new QLabel;
    label->setObjectName(policy.name);
    if (policy.isWriteable)
        label->setStyleSheet("color:red;");

    label->setText(processPolicyValue(policy));
    label->setIndent(50);
    return label;
}

DisplayCorePage::DisplayCorePage(QWidget *parent) : QWidget(parent)
{
    core = &cpuInfo.cores[0];

    infoList = new QWidget();
    infoLayout = new QFormLayout(infoList);
    QScrollArea *area = new QScrollArea(this);
    area->setWidget(infoList);
    for (const auto &item : core->policies)
    {
        infoLayout->addRow(item.name, getPolicyValueWidget(item));
    }
    infoLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);
    infoList->setLayout(infoLayout);
    infoList->adjustSize();
    area->resize(400,500);
    area->setGeometry(100, 50,350,600);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=]{
        core->update();
        qDebug() << core->findPolicyByName(KnownCPUPolicy::scaling_cur_freq).value;
        for (const auto &policy : core->policies)
        {
            this->updatePolicyValue(policy);
        }
    });
    timer->start(1000);
}

