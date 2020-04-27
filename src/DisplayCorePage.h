#ifndef DISPLAYCOREPAGE_H
#define DISPLAYCOREPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QFormLayout>
#include <QTreeWidgetItem>

class CPUCore;

class DisplayCorePage : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayCorePage(QWidget *parent = nullptr);

private:
    //QTreeWidget *infoList;
    QFormLayout *mainLayout;
    CPUCore *core;
    QWidget* processPolicyValue(const QString &name);
signals:

public slots:
};

#endif // DISPLAYCOREPAGE_H
