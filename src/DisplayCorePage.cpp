#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "DisplayCorePage.h"
#include "CPUInfo.h"
#include "PolicyDisplayWidget.h"
#include "PolicyEditorWidget.h"


DisplayCorePage::DisplayCorePage(CPUCore& _core, QWidget *parent) : QWidget(parent)
{

    this->core = &_core;

    monitor = new PolicyDisplayWidget(_core, this);
    monitor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editor = new PolicyEditorWidget(_core, this);
    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->addStretch(1);
    mainLayout->addWidget(monitor, 5);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(editor, 6);
    mainLayout->addStretch(1);

}

void DisplayCorePage::updateInfo()
{
    editor->updateInfo();
}

void DisplayCorePage::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

