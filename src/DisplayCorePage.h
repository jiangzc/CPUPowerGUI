#ifndef DISPLAYCOREPAGE_H
#define DISPLAYCOREPAGE_H

#include <QWidget>

class CPUPolicy;
class CPUCore;
class PolicyEditorWidget;
class PolicyDisplayWidget;

class DisplayCorePage : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayCorePage(CPUCore& _core, QWidget *parent = nullptr);
    void updateInfo();
protected:


private:
    CPUCore *core;
    PolicyEditorWidget *editor;
    PolicyDisplayWidget *monitor;


signals:

public slots:
};

#endif // DISPLAYCOREPAGE_H
