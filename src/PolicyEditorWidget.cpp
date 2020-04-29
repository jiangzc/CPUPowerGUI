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
#include <QListView>
#include "PolicyEditorWidget.h"
#include "CPUInfo.h"
#include "imageswitch.h"

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
            cpuSwitch->setChecked(!checked);
    });
    firstLine->addWidget(cpuID, Qt::AlignBaseline);
    firstLine->addWidget(cpuSwitch, Qt::AlignBaseline);

    popLabel = new QLabel;
    font = popLabel->font();
    font.setPixelSize(20);
    popLabel->setFont(font);
    popLabel->setText("");

    editorLayout->setRowMinimumHeight(row, 40);
    editorLayout->addLayout(firstLine, row, 0);
    editorLayout->addWidget(popLabel, row, 1, Qt::AlignCenter);

    row++;
    for (const auto &policy : core->policies)
    {
        if (policy.isWriteable)
        {
            QLabel *label = new QLabel;
            label->setMaximumWidth(200);
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            label->setText(policy.name);
            QWidget *widget = getEdiorPolicyValueWidget(policy);
            if (widget != nullptr)
            {
                editorLayout->setRowMinimumHeight(row, 40);
                widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                widget->setFixedHeight(40);
                editorLayout->addWidget(label, row, 0);
                editorLayout->addWidget(widget, row, 1);
                editorLayout->setRowMinimumHeight(row, 20);
                row++;
            }
        }
    }

    QPushButton *applyButton = new QPushButton;
    applyButton->setText("Apply");
    applyButton->setFixedWidth(120);
    applyButton->setFixedHeight(40);
    editorLayout->addWidget(applyButton, row+ 1, 1, 3, 1, Qt::AlignBottom | Qt::AlignRight);
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
    return QSize(500, 400);
}

void PolicyEditorWidget::popMessage(bool ok, QString msg)
{
    QPalette palette = popLabel->palette();
    if (ok)
        palette.setColor(QPalette::ColorRole::WindowText, QColor("#009900"));
    else
        palette.setColor(QPalette::ColorRole::WindowText, QColor("#CC0000"));
    popLabel->setPalette(palette);
    if (msg.isEmpty())
        msg = ok ? "yes" : "no";
    popLabel->setText(msg);
}

QWidget *PolicyEditorWidget::getEdiorPolicyValueWidget(const CPUPolicy &policy)
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
        ret->setMaximumWidth(200);
    }
    else if (policy.name == KnownCPUPolicy::scaling_max_freq || policy.name == KnownCPUPolicy::scaling_min_freq)
    {
        QSlider *slider = new QSlider(Qt::Horizontal);
        slider->installEventFilter(this);
        return slider;
    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText(policy.value);
        ret = lineEdit;
        ret->setMaximumWidth(200);
    }
    return ret;

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
