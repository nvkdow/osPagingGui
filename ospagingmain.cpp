#include "ospagingmain.h"
#include "ui_ospagingmain.h"

OsPagingMain::OsPagingMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OsPagingMain)
{
    ui->setupUi(this);
}

OsPagingMain::~OsPagingMain()
{
    delete ui;
}
