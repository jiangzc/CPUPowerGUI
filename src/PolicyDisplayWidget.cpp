#include <QScrollArea>
#include <QLabel>
#include <QEvent>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFormLayout>

#include "PolicyDisplayWidget.h"
#include "CPUInfo.h"

PolicyDisplayWidget::PolicyDisplayWidget(CPUCore &_core, QWidget *parent) : QScrollArea(parent)
{
    this->setObjectName("PolicyDisplayWidget");
    this->core = &_core;

    // 展示CPU频率信息控件
    infoList = new QWidget();
    infoList->setObjectName("infoList");

    // 控件内布局
    infoLayout = new QFormLayout(infoList);
    for (const auto &item : core->policies)
    {
        if (!item.name.isEmpty())
            infoLayout->addRow(item.name, getInfoListPolicyValueWidget(item));
    }
    infoLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);
    infoList->setLayout(infoLayout);
    infoList->adjustSize();

    this->setWidget(infoList);

}

void PolicyDisplayWidget::updateInfo()
{
    core->update();
    for (const auto &policy : core->policies)
    {
        QLabel *label = infoList->findChild<QLabel*>(policy.name, Qt::FindDirectChildrenOnly);
        if (label != nullptr)
            label->setText(processPolicyValue(policy));
    }
}


QString PolicyDisplayWidget::processPolicyValue(const CPUPolicy &policy)
{
    static QStringList freqField = {
        KnownCPUPolicy::bios_limit,
        KnownCPUPolicy::cpuinfo_cur_freq,
        KnownCPUPolicy::cpuinfo_min_freq,
        KnownCPUPolicy::cpuinfo_max_freq,
        KnownCPUPolicy::scaling_cur_freq,
        KnownCPUPolicy::scaling_min_freq,
        KnownCPUPolicy::scaling_max_freq,
        };

    QString res = policy.value;
    if (freqField.contains(policy.name))
    {
        res = QString("%0 MHz").arg(res.toInt() / 1000);
    }
    else if(policy.name == "energy_performance_available_preferences" || policy.name == KnownCPUPolicy::scaling_available_governors)
    {
        res.replace(" ", "\n");
    }
    else if (policy.name == KnownCPUPolicy::scaling_available_frequencies)
    {
        auto freqList = res.split(" ", QString::SplitBehavior::SkipEmptyParts);
        res.clear();
        for (const auto &item : freqList)
        {
            res.append(QString("%0 MHz\n").arg(item.toInt() / 1000));
        }
        res.remove(res.count() - 1, 1);
    }
    return res;
}


QWidget* PolicyDisplayWidget::getInfoListPolicyValueWidget(const CPUPolicy &policy)
{
    QLabel *label = new QLabel;
    label->setObjectName(policy.name);
    if (policy.isWriteable)
        label->setStyleSheet("color:red;");

    label->setText(processPolicyValue(policy));
    label->setIndent(50);
    return label;
}

QSize PolicyDisplayWidget::sizeHint() const
{
    return QSize(400,500);
}



