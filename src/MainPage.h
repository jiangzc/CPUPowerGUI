#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>

class CPUInfo;
class QLabel;
class QPixmap;
class QTimer;
class SwitchHeader;
class QScrollArea;

class MainPage : public QWidget
{
    Q_OBJECT
public:
    explicit MainPage(CPUInfo &cpuInfo, QWidget *parent = nullptr);

signals:


private:
    CPUInfo &m_cpuInfo;
    QLabel *nameLabel;
    QLabel *overview;
    QTimer *timer;
    QPixmap cpuLogo;
    SwitchHeader *modeGovern;
    QScrollArea *area;
    void paintEvent(QPaintEvent *event);
    QSize sizeHint() const;
    virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void updateOverview();


public slots:
};

#endif // MAINPAGE_H
