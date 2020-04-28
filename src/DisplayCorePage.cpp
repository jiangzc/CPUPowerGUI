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

void DisplayCorePage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
}

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
    background.load(":/res/pic/cpu_blue.jpg");

    core = &cpuInfo.cores[0];

    infoList = new QWidget();
    QPalette palette = infoList->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //设置背景黑色
    infoList->setAutoFillBackground(true);
    infoList->setPalette(palette);

    infoLayout = new QFormLayout(infoList);
    area = new QScrollArea(this);

    palette = area->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //设置背景黑色
    area->setAutoFillBackground(true);
    area->setPalette(palette);

   // area->verticalScrollBar()->setStyleSheet(
       QString qss1 = R"(
QScrollBar:vertical
{
    width:10px;
    background:rgba(0,0,0,0%);
    margin:0px,0px,0px,0px;
    padding-top:10px;
    padding-bottom:10px;
}
QScrollBar::handle:vertical
{
    width:10px;
    background:rgba(0,0,0,25%);
    border-radius:4px;
    min-height:40;
}
QScrollBar::handle:vertical:hover
{
    width:10px;
    background:rgba(0,0,0,50%);
    border-radius:4px;
    min-height:40;
}
QScrollBar::add-line:vertical

{
background:rgba(0,0,0,0%);
height:10px;width:10px;
subcontrol-position:bottom;

}

QScrollBar::sub-line:vertical

{
background:rgba(0,0,0,0%);
height:10px;width:10px;
subcontrol-position:top;
}

)";

       QString qss2 = R"(
QScrollBar:horizontal
{
    height:10px;
    background:rgba(0,0,0,0%);
    margin:0px,0px,0px,0px;
    padding-left:10px;
    padding-right:10px;
}
QScrollBar::handle:horizontal
{
    height:10px;
    background:rgba(0,0,0,25%);
    border-radius:4px;
    min-width:40;
}
QScrollBar::handle:horizontal:hover
{
    height:10px;
    background:rgba(0,0,0,50%);
    border-radius:4px;
    min-width:40;
}

QScrollBar::add-line:horizontal

{
background:rgba(0,0,0,0%);
height:10px;width:10px;
subcontrol-position:left;

}

QScrollBar::sub-line:horizontal

{
background:rgba(0,0,0,0%);
height:10px;width:10px;
subcontrol-position:right;
}



)";
   area->verticalScrollBar()->setStyleSheet(qss1);
   area->horizontalScrollBar()->setStyleSheet(qss2);
    area->setWidget(infoList);
    for (const auto &item : core->policies)
    {
        infoLayout->addRow(item.name, getPolicyValueWidget(item));
    }
    infoLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);
    infoList->setLayout(infoLayout);
    infoList->adjustSize();

    area->resize(400,500);
    area->setGeometry(100, 50,250,600);
    area->installEventFilter(this);
    area->setFrameStyle(QFrame::NoFrame);

    infoList->setContentsMargins(5,5,5,5);

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

