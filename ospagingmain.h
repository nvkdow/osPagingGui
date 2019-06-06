#ifndef OSPAGINGMAIN_H
#define OSPAGINGMAIN_H

#include <QMainWindow>

namespace Ui {
class OsPagingMain;
}

class OsPagingMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit OsPagingMain(QWidget *parent = nullptr);
    ~OsPagingMain();

private:
    Ui::OsPagingMain *ui;
};

#endif // OSPAGINGMAIN_H
