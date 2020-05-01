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

    monitor->move(0, 0);
    editor->move(500, 0);
    resize(1100,600);
}

