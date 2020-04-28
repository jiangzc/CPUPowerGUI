#include <QScrollArea>
#include <QTimer>
#include <QLabel>
#include <QScrollBar>
#include <QEvent>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QSlider>
#include <QComboBox>
#include <QBrush>
#include <QLineEdit>
#include <QListView>
#include <QFormLayout>
#include <QGridLayout>
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
            res.append(QString("%0 MHz\n").arg(item.toInt() / 1000));
        }
        res.remove(res.count() - 1, 1);
    }
    return res;
}

void DisplayCorePage::updateInfoListPolicyValue(const CPUPolicy &policy)
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

QWidget* DisplayCorePage::getInfoListPolicyValueWidget(const CPUPolicy &policy)
{
    QLabel *label = new QLabel;
    label->setObjectName(policy.name);
    if (policy.isWriteable)
        label->setStyleSheet("color:red;");

    label->setText(processPolicyValue(policy));
    label->setIndent(50);
    return label;
}

QWidget* DisplayCorePage::getEdiorPolicyValueWidget(const CPUPolicy &policy)
{
    QWidget *ret = nullptr;
    if (policy.name == KnownCPUPolicy::scaling_governor)
    {
        QComboBox *comboBox = new QComboBox;
        comboBox->setView(new QListView());
        auto governors = core->findPolicyByName(KnownCPUPolicy::scaling_available_governors).value.split(" ", QString::SplitBehavior::SkipEmptyParts);
        for (int i = 0; i < governors.count(); i++)
        {
            comboBox->addItem(governors[i]);
            if (policy.value == governors[i])
                comboBox->setCurrentIndex(i);
        }
        ret = comboBox;
    }
    else if (policy.name == KnownCPUPolicy::scaling_max_freq)
    {
        QSlider *slider = new QSlider(Qt::Horizontal);
        return slider;
    }
    else if (policy.name == KnownCPUPolicy::scaling_min_freq)
    {
        QSlider *slider = new QSlider(Qt::Horizontal);
        return slider;
    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText(policy.value);
        ret = lineEdit;
    }
    return ret;
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
        infoLayout->addRow(item.name, getInfoListPolicyValueWidget(item));
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
    area->setGeometry(50, 50,380,500);
    area->installEventFilter(this);
    area->setFrameStyle(QFrame::NoFrame);

    // 编辑器控件
    editor = new QWidget(this);
    palette = infoList->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    editor->setAutoFillBackground(true);
    editor->setPalette(palette);
    editor->setContentsMargins(5,5,5,5);

    // 控件内布局
    editorLayout = new QGridLayout(editor);
    for (const auto &item : core->policies)
    {
        if (item.isWriteable)
        {
            QWidget *widget = getEdiorPolicyValueWidget(item);
            if (widget != nullptr)
                editorLayout->addWidget(widget);
        }

    }
    editor->setLayout(editorLayout);
    editor->move(500, 50);
    editor->adjustSize();

    // 每秒刷新
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=]{
        core->update();
        for (const auto &policy : core->policies)
        {
            this->updateInfoListPolicyValue(policy);
        }
    });
    timer->start(1000);
}

