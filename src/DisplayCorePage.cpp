#include <QScrollArea>
#include "DisplayCorePage.h"
#include "CPUInfo.h"

QWidget* DisplayCorePage::processPolicyValue(const QString &name)
{
    QLabel *label = new QLabel;
    CPUPolicy policy = core->findPolicyByName(name);
    QString res = policy.value;
    if (policy.isWriteable)
        label->setStyleSheet("color:red;");
    if (name.endsWith("_freq"))
        res = QString("%0 MHz").arg(res.toInt() / 1000);
    else if(name == "energy_performance_available_preferences" || name == "scaling_available_governors")
    {
        res.replace(" ", "\n");
    }
    label->setText(res);
    label->setIndent(50);
    return label;
}

DisplayCorePage::DisplayCorePage(QWidget *parent) : QWidget(parent)
{
     CPUInfo cpuInfo;
     core = &cpuInfo.cores[0];
//    infoList = new QPlainTextEdit(this);
//    QString text;
//    text += QString("core id: %0\n").arg(cpuInfo.cores[0].core_id());
//    text += QString("isEnabled: %0\n").arg(cpuInfo.cores[0].isEnabled());
//    for (auto &item : cpuInfo.cores[0].policies)
//    {
//        QString value = processPolicyValue(item.name, item.value);
//        text += QString("%0:\t%1\n").arg(item.name, value);
//    }
//    infoList->setPlainText(text);
//    infoList->setTabStopWidth(40);
//    QFont font = infoList->font();
//    font.setPixelSize(18);
//    infoList->setFont(font);
//    infoList->move(50, 50);
//    infoList->setReadOnly(true);
//    infoList->resize(500,300);


//    infoList = new QTreeWidget(this);
//    infoList->setColumnCount(2);
//    infoList->setHeaderLabels(QStringList{QString(" Property"), QString(" Value")});
//    QTreeWidgetItem *idRow = new QTreeWidgetItem(infoList);
//    idRow->setText(0, "core id");
//    idRow->setText(1, QString::number( cpuInfo.cores[0].core_id()));

//    QTreeWidgetItem *item1 = new QTreeWidgetItem(item0);
//    item1->setText(0, "bbb");
//    item1->setText(1, "bbb");
//    // item0->addChild(item1);

//    // infoList->addTopLevelItem(item0);
//    infoList->setGeometry(30,30,300,300);
     QWidget *infoList = new QWidget();
     QScrollArea *area = new QScrollArea(this);

     area->setWidget(infoList);
     mainLayout = new QFormLayout(infoList);
     for (const auto &item : core->policies)
     {
         mainLayout->addRow(item.name, processPolicyValue(item.name));
     }
     mainLayout->setHorizontalSpacing(80);
     mainLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);
     infoList->setLayout(mainLayout);
     infoList->adjustSize();
     area->resize(400,500);
     area->setGeometry(100, 50,350,600);

}

