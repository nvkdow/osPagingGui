#include "ospagingmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OsPagingMain w;
    w.show();

    return a.exec();
}
