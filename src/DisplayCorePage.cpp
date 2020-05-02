#include <QHBoxLayout>
#include "DisplayCorePage.h"
#include "CPUInfo.h"
#include "PolicyDisplayWidget.h"
#include "PolicyEditorWidget.h"


DisplayCorePage::DisplayCorePage(CPUCore& _core, QWidget *parent) : QWidget(parent)
{

    this->core = &_core;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    monitor = new PolicyDisplayWidget(_core, this);
    editor = new PolicyEditorWidget(_core, this);

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(monitor);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(editor);
    mainLayout->addStretch();

    resize(1100,600);
}

