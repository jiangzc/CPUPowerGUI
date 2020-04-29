#ifndef POLICYEDITORWIDGET_H
#define POLICYEDITORWIDGET_H

#include <QWidget>

class CPUPolicy;
class CPUCore;
class QGridLayout;


class PolicyEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PolicyEditorWidget(CPUCore &_core, QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    virtual QSize sizeHint() const override;

private:
    CPUCore *core;
    QGridLayout *editorLayout;
    QWidget* getEdiorPolicyValueWidget(const CPUPolicy &policy);
    virtual void paintEvent(QPaintEvent *e) override;

signals:

public slots:
};

#endif // POLICYEDITORWIDGET_H
