#ifndef DISPLAYCOREPAGE_H
#define DISPLAYCOREPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QPixmap>

class CPUPolicy;
class CPUCore;
class QScrollArea;

class DisplayCorePage : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayCorePage(CPUCore& core, QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWidget *infoList;
    QFormLayout *infoLayout;
    QScrollArea *area;
    CPUCore *core;
    QTimer *timer;

    QWidget* getPolicyValueWidget(const CPUPolicy &policy);
    QString processPolicyValue(const CPUPolicy &policy);
    void updatePolicyValue(const CPUPolicy &policy);

signals:

public slots:
};

#endif // DISPLAYCOREPAGE_H
