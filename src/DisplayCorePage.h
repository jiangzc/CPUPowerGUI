#ifndef DISPLAYCOREPAGE_H
#define DISPLAYCOREPAGE_H

#include <QWidget>
#include <QFormLayout>

class CPUPolicy;
class CPUCore;

class DisplayCorePage : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayCorePage(QWidget *parent = nullptr);

private:
    QWidget *infoList;
    QFormLayout *infoLayout;
    CPUCore *core;
    QTimer *timer;
    QWidget* getPolicyValueWidget(const CPUPolicy &policy);
    QString processPolicyValue(const CPUPolicy &policy);
    void updatePolicyValue(const CPUPolicy &policy);
signals:

public slots:
};

#endif // DISPLAYCOREPAGE_H
