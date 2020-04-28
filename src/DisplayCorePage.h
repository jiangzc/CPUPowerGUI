#ifndef DISPLAYCOREPAGE_H
#define DISPLAYCOREPAGE_H

#include <QWidget>

class CPUPolicy;
class CPUCore;
class QScrollArea;
class QGridLayout;
class QFormLayout;

class DisplayCorePage : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayCorePage(CPUCore& core, QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWidget *infoList;
    QWidget *editor;
    QGridLayout *editorLayout;
    QFormLayout *infoLayout;
    QScrollArea *area;
    CPUCore *core;
    QTimer *timer;

    QWidget* getInfoListPolicyValueWidget(const CPUPolicy &policy);
    QString processPolicyValue(const CPUPolicy &policy);
    void updateInfoListPolicyValue(const CPUPolicy &policy);

    QWidget* getEdiorPolicyValueWidget(const CPUPolicy &policy);


signals:

public slots:
};

#endif // DISPLAYCOREPAGE_H
