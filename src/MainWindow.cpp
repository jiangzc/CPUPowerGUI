#include <QPainter>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "MainWindow.h"
#include "MainPage.h"
#include "DisplayCorePage.h"
#include "SwitchHeader.h"
#include "CPUInfo.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    background.load(":/res/pic/cpu_blue.jpg");
    mainLayout = new QVBoxLayout(this);
    header = new SwitchHeader;
    header->append("All");
    for (int i = 0; i < CPUInfo::instance().cores.count(); i++)
    {
        header->append("CPU " + QString::number(i));
    }
    header->adjustSize();
    mainLayout->addSpacing(20);
    header->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(header, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);

    stackLayout = new QStackedLayout;
    auto mainPage = new MainPage(CPUInfo::instance());
    stackLayout->addWidget(mainPage);
    for (auto &core : CPUInfo::instance().cores)
    {
        auto corePage = new DisplayCorePage(core);
        stackLayout->addWidget(corePage);
    }
    mainLayout->addLayout(stackLayout);
    mainLayout->addSpacing(20);
    resize(1200, 700);
    connect(header, &SwitchHeader::indexChanged, stackLayout, [=](int index){
        if (index == 0)
        {
            MainPage *p1 = dynamic_cast<MainPage*>(stackLayout->widget(index));
            p1->updateInfo();
        }
        stackLayout->setCurrentIndex(index);
    });
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
}
