#include "ospagingmain.h"
#include "ui_ospagingmain.h"
#include <QMessageBox>

#include <algorithm>
#include <random>

#define PAGE_SIZE 20
#define MULTI_PROG_DEGREE 20

//==================== MAIN FUNCTIONS ====================//

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

// MAIN LOGIC
void OsPagingMain::on_pushButton_clicked()
{
    // Check some conditions to add new program
    if(ui->programInfo1->text().isEmpty()
            || ui->programInfo2->text().isEmpty()) {
        QMessageBox::information(this, tr("alert"), tr("모든 정보를 기입해주세요!"));
        return;
    }
    else if(ui->tablePrograms->rowCount() == MULTI_PROG_DEGREE) {
        QMessageBox::information(this, tr("alert"), tr("프로그램 적재량이 꽉 찼습니다!"));
        return;
    }
    else if(isRangeOverlapped(ui->programInfo1->text(), ui->programInfo2->text())) {
        QMessageBox::information(this, tr("alert"), tr("다른 프로그램의 주소를 침범하는 정보입니다!"));
        return;
    }

    // LOGIC #1 : ADD INFO TO 'Programs' TABLE
    // Get data from 'edit line' in 'New Program' tab
    QString progAddr = ui->programInfo1->text();
    QTableWidgetItem* item1 = new QTableWidgetItem(progAddr);
    item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
    item1->setTextAlignment(Qt::AlignCenter);
    QString progSize = ui->programInfo2->text();
    QTableWidgetItem* item2 = new QTableWidgetItem(progSize);
    item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
    item2->setTextAlignment(Qt::AlignCenter);

    // Insert row to 'Programs' table
    int insertedRow = ui->tablePrograms->rowCount();
    ui->tablePrograms->insertRow(insertedRow);
    ui->tablePrograms->setItem(insertedRow, 0, item1);
    ui->tablePrograms->setItem(insertedRow, 1, item2);



    // LOGIC #2 : ASSIGN SOME PAGE to PROGRAM
    // Decide how many page will be created
    std::random_device rd;
    std::mt19937 prng(rd());
    std::uniform_int_distribution<__int64> distribution(0, std::min(10, progSize.toInt() / PAGE_SIZE));
}

//==================== ADDITIONAL FUNCTIONS ====================//

bool isNum(QString str) {
    bool rtn = true;
    for(auto chr : str) {
        if(chr < '0' || chr > '9')
            rtn = false;
    }
    return rtn;
}
bool OsPagingMain::isRangeOverlapped(QString strA1, QString strA2) {
    int a1 = strA1.toInt();
    int a2 = strA2.toInt();
    bool isOverlapped = false;

    for(int i = 0; i < ui->tablePrograms->rowCount(); i++) {
        int b1 = ui->tablePrograms->item(i, 0)->text().toInt();
        int b2 = ui->tablePrograms->item(i, 1)->text().toInt();
        if(std::max(a1, b1) < std::min(a1 + a2, b1 + b2)) {
            isOverlapped = true;
            return isOverlapped;
        }
    }
    return isOverlapped;
}

void OsPagingMain::on_programInfo1_editingFinished()
{
    if(!isNum(ui->programInfo1->text())) {
        QMessageBox::information(this, tr("alert"), tr("0 이상의 숫자만을 기입해주세요!"));
        ui->programInfo1->clear();
    }
}
void OsPagingMain::on_programInfo2_editingFinished()
{
    if(!isNum(ui->programInfo2->text()) || ui->programInfo2->text() == '0') {
        QMessageBox::information(this, tr("alert"), tr("0보다 큰 숫자만을 기입해주세요!"));
        ui->programInfo2->clear();
    }
}
