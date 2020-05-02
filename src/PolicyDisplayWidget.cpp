#include <QScrollArea>
#include <QTimer>
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
    this->core = &_core;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    // 展示CPU频率信息控件
    infoList = new QWidget();
    infoList->setAutoFillBackground(true);
    infoList->setPalette(palette); //透明背景
    infoList->setContentsMargins(5,5,5,5);


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
    this->setFrameStyle(QFrame::NoFrame);
    QScrollArea::installEventFilter(this);

    // 每秒刷新
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]{
        core->update();
        for (const auto &policy : core->policies)
        {
            this->updateInfoListPolicyValue(policy);
        }
    });
    timer->start(1000);

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

void PolicyDisplayWidget::updateInfoListPolicyValue(const CPUPolicy &policy)
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

bool PolicyDisplayWidget::eventFilter(QObject *obj, QEvent *event)
{
    QPainter painter;
    if (obj == this && event->type() == QEvent::Paint)
    {
        painter.begin(this);
        QBrush brush(QColor(222,222,222,222));
        painter.setPen(Qt::NoPen);
        painter.setBrush(brush);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawRoundedRect(this->rect(), 12, 12);
        return true;
    }
    else
        return QScrollArea::eventFilter(obj, event);
}

