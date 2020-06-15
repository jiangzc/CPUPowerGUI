#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "UpdateInfoInterface.h"
class CPUInfo;
class QLabel;
class QPixmap;
class SwitchButton;
class QScrollArea;

class MainPage : public QWidget, public UpdateInfoInterface
{
    Q_OBJECT
public:
    explicit MainPage(CPUInfo &cpuInfo, QWidget *parent = nullptr);
    void updateInfo() override;
    void updateGovern();
signals:


private:
    CPUInfo &m_cpuInfo;
    QLabel *nameLabel;
    QLabel *overview;

    QPixmap cpuLogo;
    SwitchButton *modeGovern;
    QScrollArea *area;
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;
    int detectCurrentGovern();
    void setCurrentGovern(int govern);

private slots:
    void updateOverview();


public slots:
};

#endif // MAINPAGE_H
