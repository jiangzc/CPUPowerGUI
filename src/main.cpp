#include "DisplayUI.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DisplayUI w;
    w.show();

    return a.exec();
}
