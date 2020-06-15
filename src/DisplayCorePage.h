#ifndef DISPLAYCOREPAGE_H
#define DISPLAYCOREPAGE_H

#include <QWidget>
#include "UpdateInfoInterface.h"

class CPUPolicy;
class CPUCore;
class PolicyEditorWidget;
class PolicyDisplayWidget;

class DisplayCorePage : public QWidget, public UpdateInfoInterface
{
    Q_OBJECT
public:
    explicit DisplayCorePage(CPUCore& _core, QWidget *parent = nullptr);
    void updateInfo() override;
protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    CPUCore *core;
    PolicyEditorWidget *editor;
    PolicyDisplayWidget *monitor;


signals:

public slots:
};

#endif // DISPLAYCOREPAGE_H
