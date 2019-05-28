#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    one.rowCount=8;
    one.columnCount=7;
    for (int i=0; i<3; i++){
        QTableWidgetItem *item = new QTableWidgetItem("x"+QString::number(i+1)); //заполняем х1-x3
        //ui->tableWidget->setHorizontalHeaderItem(i, item);
        one.Horizontal_H[i].name = item->text();
        for (int j=i; j<3; j++){                                                 //заполняем х1х2-х2х3 (до разряда)
            if (i!=j){
                QTableWidgetItem *item = new QTableWidgetItem("x"+QString::number(i+1)+"x"+QString::number(j+1));
                //ui->tableWidget->setHorizontalHeaderItem(i+j+2, item);//+разряд-1
                one.Horizontal_H[i+j+2].name = item->text();
                for (int k=0; k<one.rowCount; k++){               //заполняем строки х1х2-х2х3
                    QString *str1 = new QString (to_Bin(k, 3)[i]);
                    QString *str2 = new QString (to_Bin(k, 3)[j]);
                    QString *str = new QString (QString::number(to_Dec(*str1+*str2)));
                    //ui->tableWidget->setItem(k, i+j+2, new QTableWidgetItem(*str));
                    one.tbl[k][i+j+2].x = *str;
                }
            }
        }
        for (int j=0; j<one.rowCount; j++){                       //заполняем строки x1-x3
            QString *str = new QString (to_Bin(j, 3)[i]);
            //ui->tableWidget->setItem(j, i, new QTableWidgetItem(*str));
            one.tbl[j][i].x = *str;
        }
        //заполняем строки х1х2-х2х3

    }
    QTableWidgetItem *item = new QTableWidgetItem("x1x2x3");
    //ui->tableWidget->setHorizontalHeaderItem(ui->tableWidget->columnCount()-1, item);
    one.Horizontal_H[6].name = item->text();

    for (int j=0; j<one.rowCount; j++){                           //заполняем строки m0-m8+столбец х1х2х3
        QTableWidgetItem *item = new QTableWidgetItem("m"+QString::number(j));
        //ui->tableWidget->setVerticalHeaderItem(j, item);
        one.Vertical_H[j].name = item->text();
        //ui->tableWidget->setItem(j, ui->tableWidget->columnCount()-1, new QTableWidgetItem(QString::number(j)));
        one.tbl[j][6].x = QString::number(j);
    }

    //Update1();
}
MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::Next(QString s, QString s1){
    if(s.contains("-"))
        return "Нет";
    else{
        if (s.contains("x"))
            s = "-" + s;
        else
            s = "x" + s1;
    }
    return s;
}
void MainWindow::otvetUpdate(){
    ui->otvet->setText(Sx1);
    if((Sx1!="")&&(Sx2!=""))
        ui->otvet->setText(ui->otvet->text());
    ui->otvet->setText(ui->otvet->text()+Sx2);
    if((ui->otvet->text()!="")&&(Sx3!=""))
        ui->otvet->setText(ui->otvet->text());
    ui->otvet->setText(ui->otvet->text()+Sx3);
    if (ui->otvet->text()!="")
        ui->otvet->setText("("+ui->otvet->text()+")");
}
void MainWindow::on_pushButton_2_clicked()
{
    Sx1 = (ui->pushButton_2->text()=="Нет")?"":ui->pushButton_2->text();
    ui->pushButton_2->setText(Next(Sx1, "1"));
    otvetUpdate();
}
void MainWindow::on_pushButton_clicked()
{
    Sx2 = (ui->pushButton->text()=="Нет")?"":ui->pushButton->text();
    ui->pushButton->setText(Next(Sx2, "2"));
    otvetUpdate();
}
void MainWindow::on_pushButton_3_clicked()
{
    Sx3 = (ui->pushButton_3->text()=="Нет")?"":ui->pushButton_3->text();
    ui->pushButton_3->setText(Next(Sx3, "3"));
    otvetUpdate();
}

bool MainWindow::isFindHorizontalHeader(QString s){
    for (int i=0; i<ui->tableWidget->columnCount(); i++)
        if (ui->tableWidget->horizontalHeaderItem(i)->text() == s)
            return true;
    return false;
}

bool MainWindow::isFindVerticalHeader(QString s){
    for (int i=0; i<ui->tableWidget->rowCount(); i++)
        if (ui->tableWidget->verticalHeaderItem(i)->text() == s)
            return true;
    return false;
}

void MainWindow::Change_Table(QString s){
    s = s.remove("(");
    s = s.remove(")");
    s = s.remove("+");
    //определяем колонку
    int iCol=(s.contains("1")?1:0)+(s.contains("2")?2:0)+(s.contains("3")?3:0);
    if (s.length()<4)
        iCol--;
    //преобразовываем строку в число (Дес)
    QString *str = new QString ();
    *str += new QString((s.contains("-x1"))?"0":((s.contains("x1"))?"1":""));
    *str += new QString((s.contains("-x2"))?"0":((s.contains("x2"))?"1":""));
    *str += new QString((s.contains("-x3"))?"0":((s.contains("x3"))?"1":""));
    int iDec = to_Dec(*str);
    delete str;
    //находим iDec в столбце для смены bgcol во всей строке (вычёркиваем её, помечая в другом месте, что это число больше не исп)
    for (int i=0; i<8;i++){
        if (one.tbl[i][iCol].x == QString::number(iDec)){
        //if (ui->tableWidget->item(i, iCol)->text() == QString::number(iDec)){
                //QColor *col1 = new QColor(0, 180, 0);
                for (int j=0; j<7; j++){
                    //QColor *col = new QColor(0, 255, 0);
                    //if (iCol == j)
                        //*col=*col1;
                    one.tbl[i][j].isHere = true; //меняем параметр наличия числа в таблице на тру
                    //if (ui->tableWidget->item(i, j)->backgroundColor() != *col1)
                        //ui->tableWidget->item(i, j)->setBackgroundColor(*col);//меняем цвет ячейки
                }
                //vec.append(i);
        }
    }

    if (!(isFindHorizontalHeader(s))){
        ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()+1);
        ui->tableWidget->setHorizontalHeaderItem(ui->tableWidget->columnCount()-1, new QTableWidgetItem(s));
    }


}
double MainWindow::to_Dec(QString s)
{
   if (s!=""){
        double n = 0;
        for (int i=0; i<s.length(); i++)
            if (s[i] == "1")
                n+=exp((s.length()-1-i)*log(2));
        return n;
   }else
       return -1;
}
QString MainWindow::to_Bin(int t, int r=3)
{
        QString str = "";
        do{
            str = str + QString::number(t % 2);
            t/=2;
        } while(t!=0);
        while (str.length()<r)
            str+="0";
        return revers(str);
}
void MainWindow::Update1(){
    QColor *check = new QColor (255, 255, 255);
    for (int i=0; i<ui->tableWidget->rowCount(); i++)
        for (int j=0; j<ui->tableWidget->columnCount(); j++){
            ui->tableWidget->item(i, j)->setBackgroundColor(*check);
            QColor *ch = new QColor(0, 0, 0);
            ui->tableWidget->item(i, j)->setTextColor(*ch);
        }
}
QString MainWindow::revers(QString str)
{
        QString str1 = "";
        for (int i=str.length()-1; i>=0; i--)
                str1+=str[i];
        return str1;
}
void MainWindow::on_pushButton_5_clicked()
{
    if (!(ui->sdnf->text().contains(ui->otvet->text())))
    {
        if (ui->sdnf->text()!="")
            ui->sdnf->setText(ui->sdnf->text()+"+");
        ui->sdnf->setText(ui->sdnf->text()+ui->otvet->text());
    }
    ui->pushButton_2->setText("x1"); Sx1="";
    ui->pushButton->setText("x2"); Sx2="";
    ui->pushButton_3->setText("x3"); Sx3="";
    if (ui->otvet->text()!="")
        Change_Table(ui->otvet->text());
    ui->otvet->clear();
}

QString MainWindow::Change_Table_Inverse(table t, int i, int j){
    QString str = "";
    QString s = to_Bin(t.tbl[i][j].x.toInt(), (j/3)+1);
    //QString s = to_Bin(ui->tableWidget->item(i, j)->text().toInt(), (j/3)+1);
    QString s1 = t.Horizontal_H[j].name;
    //QString s1 = ui->tableWidget->horizontalHeaderItem(j)->text();
    for (int k=0; k<s.length(); k++){
        str += (s[k]=="0")?"-":"";
        str.push_back(s1[2*k]);
        str.push_back(s1[2*k+1]);
    }
    return str;
}

int min (int a, int b){
    if (a<b)
        return a;
    else
        return b;
}


QString MainWindow::sokr(QString s1, QString s2){
    QString str = "";
    int it=1;
    int count = 0;
    while (it<=3){
        QString x1 = s1.left(s1.indexOf(QString::number(it))+1);
        s1.remove(x1);
        QString x2 = s2.left(s2.indexOf(QString::number(it))+1);
        s2.remove(x2);
        //ui->otvet->setText(ui->otvet->text()+x1+" "+QString::number(it)+"; ");
        if (x1 == x2){
            str +=x1;
            if (x1 != "")
                count++;
        }
        else
            if ((x1 == "")||(x2 == ""))
                count++;
        it++;
    }
    it--;
    if ((count == min(s1.length(), s2.length())) || ((s1.length() == s2.length())&&(it-1==count))){
        return str;
    }
    //QString x1 = str.left(str.indexOf("1")+1);
    //ui->knf->setText(x1);
    else
        return "";
}

void MainWindow::doTable(){
    for (int i=0; i<ui->tableWidget->columnCount()-1; i++){
        bool bFindEvolve = false;
        for (int j=i+1; j<ui->tableWidget->columnCount(); j++){
            //функция сокращения
            QString s = sokr(ui->tableWidget->horizontalHeaderItem(i)->text(), ui->tableWidget->horizontalHeaderItem(j)->text());
            if (s != "")
                if (!(isFindVerticalHeader(s))){
                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                    ui->tableWidget->setVerticalHeaderItem(ui->tableWidget->rowCount()-1, new QTableWidgetItem(s));
                    bFindEvolve = true;
                    //пометить иксик
                    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, i, new QTableWidgetItem("x"));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, j, new QTableWidgetItem("x"));
                }
        }
        if (bFindEvolve == false){
            int x = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(x+1);
            ui->tableWidget->setVerticalHeaderItem(x, ui->tableWidget->horizontalHeaderItem(i));
            ui->tableWidget->setItem(x, i, new QTableWidgetItem("x"));
        }
    }
    //проверка для последнего элемента
    bool bFindEvolve = false;
    for (int j=0; j<ui->tableWidget->columnCount()-1; j++){
        QString s = sokr(ui->tableWidget->horizontalHeaderItem(ui->tableWidget->columnCount()-1)->text(), ui->tableWidget->horizontalHeaderItem(j)->text());
        if (s != "")
                bFindEvolve = true;
    }
    if (bFindEvolve == false){
        int x = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(x+1);
        ui->tableWidget->setVerticalHeaderItem(x, new QTableWidgetItem(ui->tableWidget->horizontalHeaderItem(ui->tableWidget->columnCount()-1)->text()));
        int y = ui->tableWidget->columnCount()-1;
        QString *s = new QString("x");
        ui->tableWidget->setItem(x, y, new QTableWidgetItem(*s));
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->knf->clear();

    //Функция задания иксиков и строчек
    doTable();

    //QColor *check = new QColor (255, 255, 255);
    //QColor *vicherck= new QColor(255, 179, 0);
    //Вычёркиваем клеточки из зелёных
    for (int j=0; j<one.columnCount-1; j++)
        for (int i=0; i<one.rowCount; i++)
            if (one.tbl[i][j].isHere == false)
            //if (ui->tableWidget->item(i, j)->backgroundColor() == *check)
                for (int k=0; k<one.rowCount; k++)
                    if ((one.tbl[i][j].x == one.tbl[k][j].x)&&(one.tbl[k][j].isHere)){
                        one.tbl[k][j].isHere = false;
                    //if ((ui->tableWidget->item(i, j)->text() == ui->tableWidget->item(k, j)->text())&&(ui->tableWidget->item(k, j)->backgroundColor() != *check)){
                        //ui->tableWidget->item(k, j)->setTextColor(QColor(255, 0, 0));
                        //ui->tableWidget->item(k, j)->setBackgroundColor(*vicherck);
                    }

    //QTableWidget *tw = new QTableWidget(ui->tableWidget);
    //Вычёркиваем до конца с одной переменной
    for (int j=0; j<3; j++)
        for (int i=0; i<one.rowCount; i++)
            if (one.tbl[i][j].isHere == true)
            //if ((tw->item(i, j)->backgroundColor() != *vicherck)&&(tw->item(i, j)->backgroundColor() != *check))
                for (int k=0; k<one.columnCount; k++)
                    if (k != j){
                        one.tbl[i][k].isHere = false;
                        //ui->tableWidget->item(i, k)->setBackgroundColor(*vicherck);
                    }

    bool bFlag = true;
    while (bFlag){
        table two = one;
        //Находим повторяющиеся, их оставляем, а остальное вычёркиваем в столбце и строках
        for (int j=3; j<two.columnCount-1; j++)
            for (int i=0; i<two.rowCount-1; i++)
                if (two.tbl[i][j].isHere)
                //if ((tw->item(i, j)->backgroundColor() != *vicherck)&&(tw->item(i, j)->backgroundColor() != *check))
                    for (int k=i+1; k<two.rowCount; k++)
                        if(two.tbl[i][j].x == two.tbl[k][j].x){
                            //if (tw->item(i, j)->text() == tw->item(k, j)->text()){
                                for (int l=0; l<two.rowCount; l++)
                                    if ((two.tbl[l][j].isHere) && (l!=i) && (k!=l)){
                                    //if ((tw->item(l, j)->backgroundColor() != *check) && (l!=i) && (k!=l))
                                        two.tbl[l][j].isHere = false;
                                        //ui->tableWidget->item(l, j)->setBackgroundColor(*vicherck);
                                    }
                                for (int l=3; l<two.columnCount; l++)
                                    if (l!=j){
                                        two.tbl[i][l].isHere = false;
                                        two.tbl[k][l].isHere = false;
                                        //tw->item(i, l)->setBackgroundColor(*vicherck);
                                        //tw->item(k, l)->setBackgroundColor(*vicherck);
                                    }

                        }


        //Преобразовать оставшиеся в ДНФ
        if (ui->knf->text()!="")
            ui->knf->setText(ui->knf->text()+"; ");
        QString s="";
        for (int j=0; j<two.columnCount; j++)
            for (int i=0; i<two.rowCount; i++)
                if (two.tbl[i][j].isHere){
                    one.tbl[i][j].isHere = false;
                //if ((tw->item(i,j)->backgroundColor()!=*check)&&(tw->item(i,j)->backgroundColor()!=*vicherck)){
                    bool flag = true;
                    for (int k=0; k<=i; k++)
                        if ((k!=i)&&(two.tbl[k][j].isHere))
                        //if ((k!=i)&&(tw->item(k, j)->backgroundColor()!=*check)&&(tw->item(k, j)->backgroundColor()!=*vicherck))
                            if (j!=two.columnCount-1)
                                flag = false;
                    if (flag){
                        if (s!="")
                            s.push_back("+");
                        //ui->knf->setText(ui->knf->text()+QString::number(j));
                        s+=Change_Table_Inverse(two, i, j);
                    }
                    if ((ui->knf->text()!="")&&(j==two.columnCount-1))
                        s="";
                }
        ui->knf->setText(ui->knf->text() + s);

        if (s=="")
            bFlag = false;
    }
}
