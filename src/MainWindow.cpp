#include <QWidget>
#include <QPainter>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include "MainWindow.h"
#include "MainPage.h"
#include "DisplayCorePage.h"
#include "SwitchButton.h"
#include "CPUInfo.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    background.load(":/res/pic/cpu_blue.jpg");
    mainLayout = new QVBoxLayout(this);
    header = new SwitchButton;
    header->append("All");
    for (int i = 0; i < CPUInfo::instance().cores.count(); i++)
    {
        header->append("Core " + QString::number(i));
    }
    header->adjustSize();
    header->setCurrentIndex(0);
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
    stackLayout->setCurrentIndex(0);
    mainLayout->addLayout(stackLayout);
    mainLayout->addSpacing(20);
    resize(1200, 700);
    connect(header, &SwitchButton::indexChanged, stackLayout, [=](int index){
        if (index == 0)
        {
            MainPage *p = dynamic_cast<MainPage*>(stackLayout->widget(index));
            p->updateGovern();
        }
        stackLayout->setCurrentIndex(index);
    });

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]{
        UpdateInfoInterface *updatePage = dynamic_cast<UpdateInfoInterface *>(stackLayout->currentWidget());
        if (updatePage != nullptr)
            updatePage->updateInfo();
    });
    timer->start(1000);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
}
