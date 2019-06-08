#include "ospagingmain.h"
#include "ui_ospagingmain.h"
#include <QMessageBox>

#include <algorithm>

#define PAGE_SIZE 20
#define MAX_PAGE_NUM 10
#define MAX_PMT_SIZE 200
#define MULTI_PROG_DEGREE 20

//==================== MAIN FUNCTIONS ====================//

OsPagingMain::OsPagingMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OsPagingMain)
{
    ui->setupUi(this);

    // Initialize random number generator
    // Using Mersenne Twister
    std::random_device rnd;
    this->mPRNG = std::mt19937(rnd());

    // Initialize MainMemory Address
    for(int i = 0; i < MAX_PMT_SIZE * MULTI_PROG_DEGREE; i++) {
        QTableWidgetItem* item1 = new QTableWidgetItem("-");
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        item1->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(i));
        item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
        item2->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item3 = new QTableWidgetItem("-");
        item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
        item3->setTextAlignment(Qt::AlignCenter);

        ui->tableMainMemory->setItem(i, 0, item1);
        ui->tableMainMemory->setItem(i, 1, item2);
        ui->tableMainMemory->setItem(i, 2, item3);
    }
}

OsPagingMain::~OsPagingMain()
{
    delete ui;
}

// MAIN LOGIC : new program
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

    // ==================== MAIN LOGIC #1 ==================== //
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
    int programNum = ui->tablePrograms->rowCount();
    ui->tablePrograms->insertRow(programNum);
    ui->tablePrograms->setItem(programNum, 0, item1);
    ui->tablePrograms->setItem(programNum, 1, item2);



    // ==================== MAIN LOGIC #2 ==================== //
    // LOGIC #2 : ASSIGN PAGE INFO to PMT(Page Mapping Table)
    const int totalPageNum = (progSize.toInt() - 1) / PAGE_SIZE + 1;
    std::uniform_int_distribution<__int64> distribution1(1, std::min(MAX_PAGE_NUM, totalPageNum));
    std::uniform_int_distribution<__int64> distribution2(0, 1);
    const int initPageNum = int(distribution1(this->mPRNG));
    int scndAddr = progAddr.toInt();
    int pmtRowCount = ui->tablePMT->rowCount();

    // Add Page mapping info into PMT
    for(int pmtNum = pmtRowCount, chk = 0; pmtNum < pmtRowCount + totalPageNum; pmtNum++) {
        bool resiBitOn = false;
        QTableWidgetItem* item0 = new QTableWidgetItem(QString::number(pmtNum));
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        item0->setTextAlignment(Qt::AlignCenter);

        if(chk < initPageNum && distribution2(this->mPRNG) & 1) { resiBitOn = true; chk++; }
        QTableWidgetItem* item1 = new QTableWidgetItem(QString::number(int(resiBitOn)));
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        item1->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(scndAddr));
        item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
        item2->setTextAlignment(Qt::AlignCenter);

        QString pageNumber = (resiBitOn)? QString::number(this->nextPageNumber++) : "-";
        QTableWidgetItem* item3 = new QTableWidgetItem(pageNumber);
        item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
        item3->setTextAlignment(Qt::AlignCenter);

        ui->tablePMT->insertRow(pmtNum);
        ui->tablePMT->setItem(pmtNum, 0, item0);
        ui->tablePMT->setItem(pmtNum, 1, item1);
        ui->tablePMT->setItem(pmtNum, 2, item2);
        ui->tablePMT->setItem(pmtNum, 3, item3);

        // ==================== MAIN LOGIC #3 ==================== //
        // LOGIC #3 : ASSIGN SOME PAGE to MainMemory
        if(resiBitOn) {
            // Search for empty space in MainMemory
            int strtAddr = 0;
            for(; strtAddr < MAX_PMT_SIZE * PAGE_SIZE; strtAddr += 20) {
                if(ui->tableMainMemory->item(strtAddr, 0)->text() == "-")
                    break;
            }

            // Delete assigned page if MainMemory is full
            // Deprecated since Assignment says it doesn't need this feature  :(
            /* if(strtAddr >= 4000) {
                strtAddr = this->removePageAddr;
                removePage();
            } */

            int restSize = (progAddr.toInt() + progSize.toInt()) - scndAddr;
            std::uniform_int_distribution<__int64> valueGenerator(1, 9999);
            for(int i = 0; i < PAGE_SIZE && i < restSize; i++) {
                QTableWidgetItem* item1 = new QTableWidgetItem(pageNumber);
                item1->setFlags(item2->flags() & ~Qt::ItemIsEditable);
                item1->setTextAlignment(Qt::AlignCenter);
                QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(valueGenerator(this->mPRNG)));
                item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
                item2->setTextAlignment(Qt::AlignCenter);

                ui->tableMainMemory->setItem(strtAddr + i, 0, item1);
                ui->tableMainMemory->setItem(strtAddr + i, 2, item2);
            }
        }
        scndAddr += PAGE_SIZE;
    }
}

// MAIN LOGIC 2 : access real address
void OsPagingMain::on_pushButton_2_clicked()
{
    // Check some conditions to add new program
    if(ui->accessInfo1->text().isEmpty()
            || ui->accessInfo2->text().isEmpty()) {
        QMessageBox::information(this, tr("alert"), tr("모든 정보를 기입해주세요!"));
        return;
    }

    // ==================== MAIN LOGIC 2 #1 ==================== //
    // LOGIC #1 : ACCESS to PMT
    int pageNum = ui->accessInfo1->text().toInt();

    // ==================== MAIN LOGIC 2 #2 ==================== //
    // LOGIC #2 : Calculate Real Address
    // If residence bit == 1
    if(ui->tablePMT->item(pageNum, 1)->text() == '1') {
        // Real Address Calculation : p' * pageSize + offset
        int realAddress = ui->tablePMT->item(pageNum, 3)->text().toInt() * PAGE_SIZE + ui->accessInfo2->text().toInt();
        QString value = ui->tableMainMemory->item(realAddress, 2)->text();
        QMessageBox::information(this, "Access Success!", "Real Address : " + QString::number(realAddress) + "\nValue : " + value);
    }
    // If residence bit == 0
    else {
        ui->tablePMT->item(pageNum, 1)->setText("1");
        // Find program address, size
        int scndAddr = ui->tablePMT->item(pageNum, 2)->text().toInt();
        int progAddr, progSize;
        for(int i = 0; i < ui->tablePrograms->rowCount(); i++) {
            int a1 = ui->tablePrograms->item(i, 0)->text().toInt();
            int a2 = ui->tablePrograms->item(i, 1)->text().toInt();
            if(a1 <= scndAddr && scndAddr <= a1 + a2) {
                progAddr = a1;
                progSize = a2;
                break;
            }
        }

        // Assign new Page to PMT, MainMemory
        ui->tablePMT->item(pageNum, 3)->setText(QString::number(this->nextPageNumber));
        int restSize = (progAddr + progSize) - scndAddr;
        for(int i = 0; i < MAX_PMT_SIZE * PAGE_SIZE; i += 20) {
            if(ui->tableMainMemory->item(i, 0)->text() == "-") {
                std::uniform_int_distribution<__int64> valueGenerator(1, 9999);
                for(int j = 0; j < PAGE_SIZE && j < restSize; j++) {
                    ui->tableMainMemory->item(i + j, 0)->setText(QString::number(this->nextPageNumber));
                    ui->tableMainMemory->item(i + j, 2)->setText(QString::number(valueGenerator(this->mPRNG)));
                }
                this->nextPageNumber++;
                break;
            }
        }

        // Real Address Calculation : p' * pageSize + offset
        int realAddress = ui->tablePMT->item(pageNum, 3)->text().toInt() * PAGE_SIZE + ui->accessInfo2->text().toInt();
        QString value = ui->tableMainMemory->item(realAddress, 2)->text();
        QMessageBox::information(this, "Access Success!", "Real Address : " + QString::number(realAddress) + "\nValue : " + value);
    }
}

void OsPagingMain::removePage() {
    QString deletedPageNum = ui->tableMainMemory->item(this->removePageAddr, 0)->text();
    for(int i = this->removePageAddr; i < this->removePageAddr + PAGE_SIZE; i++) {
        ui->tableMainMemory->item(i, 0)->setText("-");
        ui->tableMainMemory->item(i, 2)->setText("-");
    }
    for(int i = 0; i < ui->tablePMT->rowCount(); i++) {
        if(ui->tablePMT->item(i, 3)->text() == deletedPageNum) {
            ui->tablePMT->item(i, 1)->setText("0");
            break;
        }
    }
    this->removePageAddr = (this->removePageAddr + PAGE_SIZE) % 4000;
}


//==================== MAIN FUNCTIONS END ====================//




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

void OsPagingMain::on_accessInfo1_editingFinished()
{
    if(!isNum(ui->accessInfo1->text()) || (ui->accessInfo1->text().toInt() >= ui->tablePMT->rowCount())) {
        QMessageBox::information(this, tr("alert"), tr("현재 PMT에 존재하는 페이지 번호를 기입해주세요!"));
        ui->accessInfo1->clear();
    }
}

void OsPagingMain::on_accessInfo2_editingFinished()
{
    if(!isNum(ui->accessInfo2->text()) || ui->accessInfo2->text().toInt() >= PAGE_SIZE) {
        QMessageBox::information(this, tr("alert"), tr("0 이상, 페이지 크기 미만의 숫자만을 기입해주세요!"));
        ui->accessInfo2->clear();
    }
}

//==================== ADDITIONAL FUNCTIONS END ====================//
