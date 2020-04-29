#ifndef POLICYEDITORWIDGET_H
#define POLICYEDITORWIDGET_H

#include <QWidget>
#include <QPair>

class CPUPolicy;
class CPUCore;
class QGridLayout;
class QLabel;


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
    QLabel *popLabel;
    void popMessage(bool ok, QString msg="");
    QGridLayout *editorLayout;
    QPair<QWidget*, QWidget*>  getEdiorPolicyValueWidget(const CPUPolicy &policy);
    virtual void paintEvent(QPaintEvent *e) override;
    bool applyChanges();

signals:

public slots:
};

#endif // POLICYEDITORWIDGET_H
