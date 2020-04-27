#include "DisplayUI.h"
#include "DisplayCorePage.h"
#include <QApplication>
#include <QColor>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font = a.font();
    font.setPixelSize(16);
    a.setFont(font);
    // a.setPalette( QPalette(QColor(222, 222, 22) ));
    DisplayCorePage w;

    w.show();
    return a.exec();
}
