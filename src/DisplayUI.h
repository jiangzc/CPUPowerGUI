#ifndef DISPLAYUI_H
#define DISPLAYUI_H

#include <QWidget>

class DisplayUI : public QWidget
{
    Q_OBJECT

public:
    DisplayUI(QWidget *parent = 0);
    ~DisplayUI();
};

#endif // DISPLAYUI_H
