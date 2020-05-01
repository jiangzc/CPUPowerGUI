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
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->setContentsMargins(5,5,5,5);

    // 控件内布局
    editorLayout = new QGridLayout;
    editorLayout->setSpacing(20);

    // 第一行： CPU编号 开启关闭按钮  消息标签
    int row = 0;
    QHBoxLayout *firstLine = new QHBoxLayout;

    QLabel *cpuID = new QLabel;
    cpuID->setText(QString("CPU %0").arg(core->core_id()));
    QFont font = cpuID->font();
    font.setPixelSize(28);
    font.setBold(true);
    cpuID->setFont(font);

    ImageSwitch *cpuSwitch = new ImageSwitch();
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

    popLabel = new QLabel;
    font = popLabel->font();
    font.setPixelSize(20);
    popLabel->setFont(font);
    popLabel->setText("");
    popLabel->setMaximumWidth(400);
    popLabel->setWordWrap(true);

    editorLayout->setRowMinimumHeight(row, 40);
    editorLayout->addLayout(firstLine, row, 0);
    editorLayout->addWidget(popLabel, row, 1, 1, 2, Qt::AlignLeft);

    row++;
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

    QPushButton *applyButton = new QPushButton;
    applyButton->setText("Apply");
    applyButton->setFixedWidth(120);
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

void PolicyEditorWidget::popMessage(bool ok, QString msg)
{
    QPalette palette = popLabel->palette();
    if (ok)
        palette.setColor(QPalette::ColorRole::WindowText, QColor("#228B22"));
    else
        palette.setColor(QPalette::ColorRole::WindowText, QColor("#CC0000"));
    popLabel->setPalette(palette);
    if (msg.isEmpty())
        msg = ok ? "Succeed" : strerror(errno);
    popLabel->setText(msg);
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
        QPalette palette = spinBox->palette();
        palette.setColor(QPalette::ColorRole::Highlight, QColor("#1ABC9C"));
        spinBox->setPalette(palette);
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
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(QColor(222,222,222,222));
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawRoundedRect(this->rect(), 12, 12);
}

bool PolicyEditorWidget::applyChanges()
{
    int succeed = 0;
    int failed = 0;
    for (const auto &item : this->children())
    {
        const auto &policy = core->policies.value(item->objectName());
        QString widgetValue;

        if (dynamic_cast<QLineEdit*>(item) != nullptr)
        {
            widgetValue = dynamic_cast<QLineEdit*>(item)->text();
        }
        if (dynamic_cast<QSlider*>(item) != nullptr)
        {
            widgetValue = QString::number(dynamic_cast<QSlider*>(item)->value() * 1000);
        }
        if (dynamic_cast<QComboBox*>(item) != nullptr)
        {
            widgetValue = dynamic_cast<QComboBox*>(item)->currentText();
        }
        if ( policy.value != widgetValue)
        {
            if (core->setPolicy(policy.name, widgetValue))
                succeed++;
            else
                failed++;
        }
    }
    if (failed == 0)
    {
        popMessage(true , QString("%0 succeed, %1 failed.").arg(succeed).arg(failed));
        return true;
    }
    else
    {
        popMessage(false , QString("%0 succeed, %1 failed. %2").arg(succeed).arg(failed).arg(strerror(errno)));
        return false;
    }

}
