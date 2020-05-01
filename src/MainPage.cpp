#include <QPainter>
#include <QBrush>
#include <QLabel>
#include <QRect>
#include <QTimer>
#include "MainPage.h"
#include "CPUInfo.h"
#include "SwitchHeader.h"

MainPage::MainPage(CPUInfo &cpuInfo, QWidget *parent) : QWidget(parent), m_cpuInfo(cpuInfo)
{
    nameLabel = new QLabel(this);
    QFont font = nameLabel->font();
    font.setPixelSize(22);
    nameLabel->setFont(font);
    nameLabel->setText(m_cpuInfo.modelName);
    nameLabel->move(250, 50);
    nameLabel->adjustSize();

    overview = new QLabel(this);
    overview->move(280,100);
    font.setPixelSize(18);
    overview->setFont(font);
    updateOverview();

    cpuLogo.load(":/res/pic/cpulogo.png");

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainPage::updateOverview);
    timer->start();

    modeGovern = new SwitchHeader(this);
    modeGovern->append("Performance");
    modeGovern->append("Fast");
    modeGovern->append("Normal");
    modeGovern->append("Slow");
    modeGovern->append("Powersave");
    modeGovern->move(250, 300);
    modeGovern->resize(700, 40);
}

void MainPage::paintEvent(QPaintEvent *)
{
    // 半透明背景
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(QColor(222,222,222,222));
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawRoundedRect(this->rect(), 12, 12);
    // CPU Logo
    painter.drawPixmap(QRect(0, 10, 250, 250), cpuLogo, cpuLogo.rect());
}

QSize MainPage::sizeHint() const
{
    return QSize(1100, 500);
}

void MainPage::updateOverview()
{
    QString info;
    info += QString("Driver: %1\n\n").arg(cpuInfo.cores[0].policies["scaling_driver"].value);
    for (auto &core : cpuInfo.cores)
    {
        info += QString("CPU %1 : isEnabled = %2,\t %3 = %4,\t %5 = %6\n")
        .arg(core.core_id()).arg(core.isEnabled())
        .arg(KnownCPUPolicy::scaling_cur_freq).arg(core.policies[KnownCPUPolicy::scaling_cur_freq].value)
        .arg(KnownCPUPolicy::cpuinfo_cur_freq).arg(core.policies[KnownCPUPolicy::cpuinfo_cur_freq].value);
    }
    overview->setText(info);
}
