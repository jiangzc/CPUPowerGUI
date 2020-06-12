#include <QLabel>
#include <QEvent>
#include <QDebug>
#include <QLineEdit>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QComboBox>
#include <QSpinBox>
#include <QListView>
#include "PolicyEditorWidget.h"
#include "CPUInfo.h"
#include "imageswitch.h"
#include <errno.h>
#include <string.h>

PolicyEditorWidget::PolicyEditorWidget(CPUCore &_core, QWidget *parent) : QWidget(parent)
{
    core = &_core;
    this->setObjectName("PolicyEditorWidget");

    // 控件内布局
    editorLayout = new QGridLayout;
    editorLayout->setSpacing(20);

    int row = 0;
    // 第一行： CPU编号 开启关闭按钮  消息标签
    initFirstLine(row);
    // 设置行：动态创建控件
    initSettingLines(row);
    // 结束行： 按钮
    QPushButton *applyButton = new QPushButton;

    applyButton->setText("Apply");
    applyButton->setFixedWidth(120);
    auto font = applyButton->font();
    font.setPixelSize(16);
    applyButton->setFont(font);
    applyButton->setFixedHeight(40);
    connect(applyButton, &QPushButton::clicked, this, &PolicyEditorWidget::applyChanges);
    editorLayout->addWidget(applyButton, row+1, 1, 3, 1, Qt::AlignBottom | Qt::AlignRight);
    this->setLayout(editorLayout);

}


bool PolicyEditorWidget::eventFilter(QObject *obj, QEvent *event)
{
    QSlider *slider = dynamic_cast<QSlider*>(obj);
    if (slider != nullptr && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
        double offset = 1.0 * mouseEvent->x() / slider->width() * (slider->maximum() - slider->minimum());
        slider->setValue( slider->minimum() + int(offset));
        return false;
    }
    return QWidget::eventFilter(obj, event);
}

QSize PolicyEditorWidget::sizeHint() const
{
    return QSize(500, 500);
}

void PolicyEditorWidget::updateInfo()
{
    cpuSwitch->setChecked(core->isEnabled());
}

void PolicyEditorWidget::popMessage(bool ok, QString msg)
{
    if (ok)
        popLabel->setStyleSheet("color: #228B22");
    else
        popLabel->setStyleSheet("color: #CC0000");

    if (msg.isEmpty())
        msg = ok ? "Succeed" : strerror(errno);
    popLabel->setText(msg);
}

void PolicyEditorWidget::initFirstLine(int &row)
{
    QHBoxLayout *firstLine = new QHBoxLayout;
    // CPU 编号
    QLabel *cpuID = new QLabel;
    cpuID->setText(QString("CPU %0").arg(core->core_id()));
    QFont font = cpuID->font();
    font.setPointSize(18);
    font.setBold(true);
    cpuID->setFont(font);
    // 选择开关
    cpuSwitch = new ImageSwitch();
    cpuSwitch->setButtonStyle(ImageSwitch::ButtonStyle_3);
    cpuSwitch->setChecked(core->isEnabled());
    cpuSwitch->setFixedHeight(30);
    connect(cpuSwitch, &ImageSwitch::checked, this, [=](bool checked){
        bool ret = core->setEnabled(checked);
        this->popMessage(ret);
        if (!ret)
        {
            cpuSwitch->setChecked(!checked);
        }
    });
    firstLine->addWidget(cpuID, Qt::AlignBaseline);
    firstLine->addWidget(cpuSwitch, Qt::AlignBaseline);
    // 提示消息
    popLabel = new QLabel;
    font = popLabel->font();
    font.setPixelSize(18);
    popLabel->setFont(font);
    popLabel->setText("");
    popLabel->setFixedSize(200, 60);
    popLabel->setWordWrap(true);

    editorLayout->setRowMinimumHeight(row, 40);
    editorLayout->addLayout(firstLine, row, 0);
    editorLayout->addWidget(popLabel, row, 1, 1, 2, Qt::AlignLeft);
    row++;
}

void PolicyEditorWidget::initSettingLines(int &row)
{
    for (const auto &policy : core->policies)
    {
        if (policy.isWriteable)
        {
            QLabel *label = new QLabel;
            label->setMaximumWidth(200);
            label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            label->setText(policy.name);
            editorLayout->setRowMinimumHeight(row, 40);
            auto widgets = getEdiorPolicyValueWidget(policy);
            QWidget *widget1 = widgets.first;
            QWidget *widget2 = widgets.second;

            if (widget1 != nullptr)
            {
                widget1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                editorLayout->addWidget(label, row, 0, Qt::AlignVCenter);
                editorLayout->addWidget(widget1, row, 1, Qt::AlignVCenter);
            }
            if (widget2 != nullptr)
            {
                widget2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                editorLayout->addWidget(widget2, row, 2, Qt::AlignVCenter);
            }
            row++;
        }
    }
}

QPair<QWidget*, QWidget*> PolicyEditorWidget::getEdiorPolicyValueWidget(const CPUPolicy &policy)
{
    QWidget *ret1 = nullptr;
    QWidget *ret2 = nullptr;
    // 下拉框
    if (policy.name == KnownCPUPolicy::scaling_governor)
    {
        QComboBox *comboBox = new QComboBox;
        comboBox->setView(new QListView());
        auto governors = core->policies.value(KnownCPUPolicy::scaling_available_governors).value.split(" ", QString::SplitBehavior::SkipEmptyParts);
        for (int i = 0; i < governors.count(); i++)
        {
            comboBox->addItem(governors[i]);
            if (policy.value == governors[i])
                comboBox->setCurrentIndex(i);
        }
        comboBox->setFixedHeight(40);
        ret1 = comboBox;
        ret1->setMaximumWidth(200);
    }
    // 滑条和数字框
    else if (policy.name == KnownCPUPolicy::scaling_max_freq || policy.name == KnownCPUPolicy::scaling_min_freq)
    {
        QSlider *slider = new QSlider(Qt::Horizontal);
        int minValue = core->policies.value(KnownCPUPolicy::cpuinfo_min_freq).value.toInt() / 1000;
        int maxValue = core->policies.value(KnownCPUPolicy::cpuinfo_max_freq).value.toInt() / 1000;
        int curValue = 0;
        if (policy.name == KnownCPUPolicy::scaling_max_freq)
        {
            curValue = core->policies[KnownCPUPolicy::scaling_max_freq].value.toInt() / 1000;
        }
        else if (policy.name == KnownCPUPolicy::scaling_min_freq)
        {
            curValue = core->policies[KnownCPUPolicy::scaling_min_freq].value.toInt() / 1000;
        }

        slider->setMinimum(minValue);
        slider->setMaximum(maxValue);
        slider->installEventFilter(this);
        slider->setFixedHeight(40);
        ret1 = slider;

        QSpinBox *spinBox = new QSpinBox;
        spinBox->setMinimum(minValue);
        spinBox->setMaximum(maxValue);
        spinBox->setFixedHeight(30);
        ret2 = spinBox;

        connect(slider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);
        connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));

        slider->setValue(curValue);
    }
    // 文本框
    else
    {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText(policy.value);
        ret1 = lineEdit;
        ret1->setMaximumWidth(200);
        ret1->setFixedHeight(40);
    }
    ret1->setObjectName(policy.name);
    return qMakePair(ret1, ret2) ;

}

void PolicyEditorWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QMap<QString, QString> PolicyEditorWidget::dumpPolicySettings()
{
    QMap<QString, QString> settings;
    for (const auto &item : this->children())
    {
        const auto &policy = core->policies.value(item->objectName());
        QString widgetValue;

        if (dynamic_cast<QLineEdit*>(item) != nullptr)
        {
            widgetValue = dynamic_cast<QLineEdit*>(item)->text();
            settings[policy.name] = widgetValue;
        }
        if (dynamic_cast<QSlider*>(item) != nullptr)
        {
            widgetValue = QString::number(dynamic_cast<QSlider*>(item)->value() * 1000);
            settings[policy.name] = widgetValue;
        }
        if (dynamic_cast<QComboBox*>(item) != nullptr)
        {
            widgetValue = dynamic_cast<QComboBox*>(item)->currentText();
            settings[policy.name] = widgetValue;
        }

    }
    return settings;
}


bool PolicyEditorWidget::applyChanges()
{
    int succeed = 0;
    int failed = 0;
    bool ret;
    auto settings = dumpPolicySettings();
    for (auto item = settings.cbegin(); item != settings.cend(); item++)
    {
        if ( core->policies[item.key()].value != item.value())
        {
            if (core->setPolicy(item.key(), item.value()))
                succeed++;
            else
                failed++;
        }
    }
    if (failed == 0)
    {
        popMessage(true , QString("%0 succeed, %1 failed.").arg(succeed).arg(failed));
        ret = true;
    }
    else
    {
        popMessage(false , QString("%0 succeed, %1 failed. %2").arg(succeed).arg(failed).arg(strerror(errno)));
        ret = false;
    }
    core->update();
    return ret;
}
