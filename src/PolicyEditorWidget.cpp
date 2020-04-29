#include <QLabel>
#include <QEvent>
#include <QDebug>
#include <QLineEdit>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFormLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QComboBox>
#include <QListView>
#include "PolicyEditorWidget.h"
#include "CPUInfo.h"

PolicyEditorWidget::PolicyEditorWidget(CPUCore &_core, QWidget *parent) : QWidget(parent)
{
    core = &_core;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->setContentsMargins(5,5,5,5);
    // this->installEventFilter(this);

    // 控件内布局
    editorLayout = new QGridLayout;

    editorLayout->setSpacing(20);

    int row = 0;
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
    QBrush brush(QColor(222,222,222,200));
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawRoundedRect(this->rect(), 12, 12);
}
