#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

struct int_bool{
    QString x;
    bool isHere = false;
};

struct Header{
    QString name;
};

struct table{
    int rowCount = 8;
    int columnCount = 7;
    Header Vertical_H[8];
    Header Horizontal_H[7];
    int_bool tbl[8][7];
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString Sx1, Sx2, Sx3;
    QVector <int>vec;
    table one;

private slots:
    void on_pushButton_2_clicked();
    QString Next(QString s, QString s1);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void otvetUpdate();

    void on_pushButton_5_clicked();

    void Change_Table(QString s);

    QString to_Bin(int t, int r);

    void Update1();

    double to_Dec(QString s);

    QString revers(QString str);

    void on_pushButton_6_clicked();

    QString Change_Table_Inverse (table t, int i, int j);

    bool isFindHorizontalHeader(QString s);

    bool isFindVerticalHeader(QString s);

    void doTable();

    QString sokr(QString s1, QString s2);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
