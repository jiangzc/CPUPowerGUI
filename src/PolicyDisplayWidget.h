#ifndef POLICYDISPLAYWIDGET_H
#define POLICYDISPLAYWIDGET_H

#include <QScrollArea>
#include "UpdateInfoInterface.h"

class CPUPolicy;
class CPUCore;
class QFormLayout;

class PolicyDisplayWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit PolicyDisplayWidget(CPUCore &_core, QWidget *parent = nullptr);
    virtual void updateInfo();

protected:
    virtual QSize sizeHint() const override;

private:
    CPUCore *core;
    QWidget *infoList;
    QFormLayout *infoLayout;
    QWidget* getInfoListPolicyValueWidget(const CPUPolicy &policy);
    QString processPolicyValue(const CPUPolicy &policy);

signals:

public slots:
};

#endif // POLICYDISPLAYWIDGET_H
