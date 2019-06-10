#include "ospagingmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OsPagingMain w;
    w.setFixedSize(1060, 540);
    w.show();

    return a.exec();
}
