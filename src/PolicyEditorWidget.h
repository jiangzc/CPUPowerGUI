#ifndef POLICYEDITORWIDGET_H
#define POLICYEDITORWIDGET_H

#include <QWidget>
#include <QPair>
#include "UpdateInfoInterface.h"

class CPUPolicy;
class CPUCore;
class QGridLayout;
class QLabel;
class ImageSwitch;

class PolicyEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PolicyEditorWidget(CPUCore &_core, QWidget *parent = nullptr);
    QMap<QString, QString> dumpPolicySettings();
    void updateInfo();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    virtual QSize sizeHint() const override;


private:
    ImageSwitch *cpuSwitch;
    CPUCore *core;
    QLabel *popLabel;                           // 提示消息的标签
    void popMessage(bool ok, QString msg="");   // 更新提示消息
    QGridLayout *editorLayout;
    void initFirstLine(int &row);               // 初始化第一行界面元素：CPU编号 开启关闭按钮  消息标签
    void initSettingLines(int &row); // 初始化设置行
    QPair<QWidget*, QWidget*>  getEdiorPolicyValueWidget(const CPUPolicy &policy); // 动态创建控件
    virtual void paintEvent(QPaintEvent *e) override;

private slots:
    bool applyChanges(); // 点击Apply按钮的槽函数

signals:

public slots:
};

#endif // POLICYEDITORWIDGET_H
