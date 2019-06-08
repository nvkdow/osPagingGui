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

private slots:
    void on_pushButton_clicked();

    void on_programInfo1_editingFinished();

    void on_programInfo2_editingFinished();

private:
    Ui::OsPagingMain *ui;

    bool isRangeOverlapped(QString strA1, QString strA2);
};

#endif // OSPAGINGMAIN_H
