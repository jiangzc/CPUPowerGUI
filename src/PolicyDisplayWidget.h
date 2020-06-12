#ifndef POLICYDISPLAYWIDGET_H
#define POLICYDISPLAYWIDGET_H

#include <QWidget>
#include <QScrollArea>

class CPUPolicy;
class CPUCore;
class QFormLayout;

class PolicyDisplayWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit PolicyDisplayWidget(CPUCore &_core, QWidget *parent = nullptr);

protected:
    virtual QSize sizeHint() const override;

private:
    CPUCore *core;
    QTimer *timer;
    QWidget *infoList;
    QFormLayout *infoLayout;
    QWidget* getInfoListPolicyValueWidget(const CPUPolicy &policy);
    QString processPolicyValue(const CPUPolicy &policy);

signals:

public slots:
};

#endif // POLICYDISPLAYWIDGET_H
