#ifndef OSPAGINGMAIN_H
#define OSPAGINGMAIN_H

#include <QMainWindow>

#include <random>

namespace Ui {
class OsPagingMain;
}

class OsPagingMain : public QMainWindow
{
    Q_OBJECT
    int removePageAddr = 0;
    int nextPageNumber = 0;

public:
    explicit OsPagingMain(QWidget *parent = nullptr);
    ~OsPagingMain();

private slots:
    void on_pushButton_clicked();

    void on_programInfo1_editingFinished();

    void on_programInfo2_editingFinished();

    void on_pushButton_2_clicked();

    void on_accessInfo1_editingFinished();

    void on_accessInfo2_editingFinished();

private:
    Ui::OsPagingMain *ui;

    // Random number generator for calculate page generation number
    std::mt19937 mPRNG;

    void removePage();
    bool isRangeOverlapped(QString strA1, QString strA2);
};

#endif // OSPAGINGMAIN_H
