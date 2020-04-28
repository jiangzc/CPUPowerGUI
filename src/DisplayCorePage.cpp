#include <QScrollArea>
#include <QTimer>
#include <QLabel>
#include <QScrollBar>
#include <QEvent>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "DisplayCorePage.h"
#include "CPUInfo.h"



bool DisplayCorePage::eventFilter(QObject *obj, QEvent *event)
 {
     if (obj == area && event->type() == QEvent::Paint)
     {
         QPainter painter(area);
         painter.setRenderHint(QPainter::Antialiasing);
         painter.setPen(Qt::NoPen);
         QBrush brush(QColor(222,222,222,200));
         painter.setBrush(brush);
         painter.drawRoundedRect(area->rect(), 12, 12);
         return true;
     }

     else
         return QWidget::eventFilter(obj, event);
 }

QString DisplayCorePage::processPolicyValue(const CPUPolicy &policy)
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
            res.append(item + "\n");
        }
        res.remove(res.count() - 1, 1);
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

DisplayCorePage::DisplayCorePage(CPUCore& _core, QWidget *parent) : QWidget(parent)
{

    this->core = &_core;

    // 展示CPU频率信息控件
    infoList = new QWidget();
    QPalette palette = infoList->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    infoList->setAutoFillBackground(true);
    infoList->setPalette(palette);
    infoList->setContentsMargins(5,5,5,5);

    // 控件内布局
    infoLayout = new QFormLayout(infoList);
    for (const auto &item : core->policies)
    {
        infoLayout->addRow(item.name, getPolicyValueWidget(item));
    }
    infoLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);
    infoList->setLayout(infoLayout);
    infoList->adjustSize();

    // 滚动区域控件
    area = new QScrollArea(this);
    palette = area->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    area->setAutoFillBackground(true);
    area->setPalette(palette);
    area->setWidget(infoList);
    area->resize(400,500);
    area->setGeometry(100, 50,380,500);
    area->installEventFilter(this);
    area->setFrameStyle(QFrame::NoFrame);



    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=]{
        core->update();
        for (const auto &policy : core->policies)
        {
            this->updatePolicyValue(policy);
        }
    });
    timer->start(1000);
}

