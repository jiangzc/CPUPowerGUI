#include <QPainter>
#include <QBrush>
#include <QLabel>
#include <QRect>
#include <QTimer>
#include <QScrollArea>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
#include <QEvent>
#include "MainPage.h"
#include "CPUInfo.h"
#include "SwitchButton.h"
#include <unistd.h>

MainPage::MainPage(CPUInfo &cpuInfo, QWidget *parent) : QWidget(parent), m_cpuInfo(cpuInfo)
{
    nameLabel = new QLabel(this);
    QFont font = nameLabel->font();
    font.setPointSize(15);
    nameLabel->setFont(font);
    nameLabel->setText(m_cpuInfo.modelName);
    nameLabel->move(290, 50);
    nameLabel->adjustSize();

    overview = new QLabel(this);
    font.setPointSize(12);
    overview->setFont(font);
    QPalette palette = overview->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    overview->setAutoFillBackground(true);
    overview->setPalette(palette);
    overview->setMargin(20);
    updateOverview();
    overview->adjustSize();

    area = new QScrollArea(this);
    palette = area->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0)); //透明背景
    area->setPalette(palette);
    area->setAutoFillBackground(true);
    area->setFrameStyle(QFrame::NoFrame);
    area->setWidget(overview);
    area->move(290, 100);
    area->setFixedSize(800,200);
    area->installEventFilter(this);

    cpuLogo.load(":/res/pic/cpulogo.png");

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainPage::updateOverview);
    timer->start();

    modeGovern = new SwitchButton(this);
    modeGovern->append("Performance");
    modeGovern->append("Fast");
    modeGovern->append("Normal");
    modeGovern->append("Slow");
    modeGovern->append("Powersave");
    modeGovern->append("Custom");
    modeGovern->move(250, 330);
    modeGovern->resize(800, 40);
    updateInfo();

    connect(modeGovern, &SwitchButton::indexChanged, this, &MainPage::setCurrentGovern);

    font.setPointSize(10);

    QPushButton *loadSettings = new QPushButton(this);
    loadSettings->setText("Load Settings");
    loadSettings->resize(150, 40);
    loadSettings->move(700, 430);
    loadSettings->setFont(font);
    connect(loadSettings, &QPushButton::clicked, this, [=]{
        QString filepath = QFileDialog::getOpenFileName(nullptr, "Load", QDir::homePath(), "*.json");
        if (!filepath.isEmpty())
            CPUInfo::instance().load(filepath);
        updateInfo();
    });

    QPushButton *saveSettings = new QPushButton(this);
    saveSettings->setText("Save Settings");
    saveSettings->resize(150, 40);
    saveSettings->move(900, 430);
    connect(saveSettings, &QPushButton::clicked, this, [=]{
        QString filepath = QFileDialog::getSaveFileName(nullptr, "Save", QDir::homePath(), "*.json");
        if (!filepath.isEmpty())
            CPUInfo::instance().dump(filepath.endsWith(".json") ? filepath : filepath + ".json");
    });
    saveSettings->setFont(font);

}

void MainPage::paintEvent(QPaintEvent *)
{
    // 半透明背景
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(QColor(222,222,222,222));
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawRoundedRect(50, 0, width()-100, height()-30, 12, 12);
    // CPU Logo
    painter.drawPixmap(QRect(50, 10, 250, 250), cpuLogo, cpuLogo.rect());
}

QSize MainPage::sizeHint() const
{
    return QSize(1100, 500);
}

bool MainPage::eventFilter(QObject *obj, QEvent *event)
{
    QPainter painter;
    if (obj == area && event->type() == QEvent::Paint)
    {
        painter.begin(area);
        QBrush brush(QColor(222,222,222,0));
        painter.setPen(Qt::NoPen);
        painter.setBrush(brush);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawRoundedRect(area->rect(), 12, 12);
        return true;
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
}

int MainPage::detectCurrentGovern()
{
    bool ok;
    PREDEFINED_GOVERNS govern;
    ok = true;
    govern = PREDEFINED_GOVERNS::Performance;
    for (auto &core : CPUInfo::instance().cores)
    {
        ok = ok && core.checkPredefinedGoverns(govern);
    }
    if (ok)
        return govern;

    govern = PREDEFINED_GOVERNS::Fast;
    ok = true;
    for (auto &core : CPUInfo::instance().cores)
    {
        ok = ok && core.checkPredefinedGoverns(govern);
    }
    if (ok)
        return govern;

    govern = PREDEFINED_GOVERNS::Normal;
    ok = true;
    for (auto &core : CPUInfo::instance().cores)
    {
        ok = ok && core.checkPredefinedGoverns(govern);
    }
    if (ok)
        return govern;

    govern = PREDEFINED_GOVERNS::Slow;
    ok = true;
    for (auto &core : CPUInfo::instance().cores)
    {
        ok = ok && core.checkPredefinedGoverns(govern);
    }
    if (ok)
        return govern;

    govern = PREDEFINED_GOVERNS::Powersave;
    ok = true;
    for (auto &core : CPUInfo::instance().cores)
    {
        ok = ok && core.checkPredefinedGoverns(govern);
    }
    if (ok)
        return govern;


    return PREDEFINED_GOVERNS::Custom;

}

void MainPage::setCurrentGovern(int govern)
{
    for (auto &core : CPUInfo::instance().cores)
    {
        core.setPredefinedGoverns(PREDEFINED_GOVERNS(govern));
    }
}

void MainPage::updateInfo()
{
    updateOverview();
    modeGovern->setCurrentIndex(detectCurrentGovern(), false);
}

void MainPage::updateOverview()
{
    QString info;
    info += QString("Driver: %1\n\n").arg(m_cpuInfo.cores[0].policies["scaling_driver"].value);
    for (auto &core : m_cpuInfo.cores)
    {
        info += QString("CPU %1 : isEnabled = %2,\t %3 = %4,\t %5 = %6\n")
        .arg(core.core_id()).arg(core.isEnabled())
        .arg(KnownCPUPolicy::scaling_cur_freq).arg(core.policies[KnownCPUPolicy::scaling_cur_freq].value)
        .arg(KnownCPUPolicy::cpuinfo_cur_freq).arg(core.policies[KnownCPUPolicy::cpuinfo_cur_freq].value);
    }
    overview->setText(info);
}
