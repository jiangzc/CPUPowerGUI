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

