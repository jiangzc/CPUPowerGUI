#include "DisplayUI.h"
#include "DisplayCorePage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DisplayCorePage w;
    w.show();

    return a.exec();
}
