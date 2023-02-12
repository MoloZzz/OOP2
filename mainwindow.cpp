#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->timeEdit->setTime(QTime::currentTime());

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()) ,this ,SLOT(TimerSlot()) );

    timer->start(1000);

    typeDate = "dd.MM.yyyy";
    typeTime = "hh:mm";
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::vector<timeT> mainList;



void MainWindow::TimerSlot()
{

date = QDate::currentDate();

time = QTime::currentTime();

time.setHMS(time.hour(),time.minute(),time.second(),0);


if(!mainList.empty()){

int pos = 0;

for(const timeT &child: mainList){
    if(child.date == date && child.time == time){

        //2 variants timer and alarm
        if(child.type == "budilnik"){
            if(child.info != ""){

              MessageBeep(MB_ICONEXCLAMATION);
              QMessageBox::about(this,"Спрацював будильник!",child.info);


            }else{

                MessageBeep(MB_ICONEXCLAMATION);
                QMessageBox::about(this,"Спрацював будильник!","Спрацював будильник без опису");

            }
        }else if(child.type == "timer"){
            if(child.info != ""){

              MessageBeep(MB_ICONEXCLAMATION);

              const char* STRopen = (child.info.toStdString()).c_str();

              system(STRopen);

              QMessageBox::about(this,"Спрацював таймер!","Відкрито файл: " + child.info);



            }else{

              MessageBeep(MB_ICONEXCLAMATION);
              QMessageBox::about(this,"Спрацював таймер!","Час вичерпано!");


            }

        }else{
            QMessageBox::warning(this,"Помилка типу","Помилка типу");

        }


        mainList.erase(mainList.begin() + pos);


        ui->listWidget->clear();


        QString outString;
        for(const timeT &print: mainList){
            if(print.type == "budilnik"){
                outString = print.date.toString(typeDate) + " " + print.time.toString(typeTime) + " " + " " + "Будильник " + print.info;

            }else if(print.type == "timer"){
                if(print.info == ""){

                outString = print.date.toString(typeDate) + " " + print.time.toString(typeTime) + " " + "Таймер";

                }else{

                outString = print.date.toString(typeDate) + " " + print.time.toString(typeTime) + " "
                        + "Таймер, відкриває файл: " + print.info;

                }
            }else{
                QMessageBox::warning(this,"Помилка типу","Помилка типу");
            }
            ui->listWidget->addItem(outString);
        }
    }
    pos++;
}

}

}



/*
void sortV(std::vector<timeT> &V){

    if(V.size() <= 1){
        return void();
    }

    int x = 0;
    timeT tmp;
    for (int i = V.size() - 1; i >= x; i--)
    {
        for (int j = V.size() - 1; j >= x; j--)    {
            if ((V[j].date < V[j-1].date) || (V[j].date <= V[j-1].date || V[j].time < V[j-1].time))    {
                tmp = V[j];
                V[j] = V[j-1];
                V[j-1] = tmp;
            }

        }
     x++;
    }


}
*/

//add budilik
void MainWindow::on_pushButton_clicked()
{

QDate Date = ui->calendarWidget->selectedDate();

QTime Time = ui->timeEdit->time();

Time.setHMS(Time.hour(),Time.minute(),0,0);

QString Info = ui->textEdit->toPlainText();

QString Type = "budilnik";


if(( Date == QDate::currentDate() && Time > QTime::currentTime() ) || ( Date > QDate::currentDate()) ){

    for(const timeT &child: mainList){

        if(child.date == Date && child.time == Time){

            QMessageBox::warning(this,"Помилка","Будильник з таким часом вже є");

            return void();

        }


    }

    timeT newNode(Time,Date,Info,Type);

    mainList.push_back(newNode);



    QMessageBox::about(this,"Додано успішно",Date.toString(typeDate) + " " + Time.toString(typeTime) + " " + Info + " " + "Будильник");

    ui->timeEdit->setTime(QTime::currentTime());
    ui->textEdit->setText("");


    QString outString = Date.toString(typeDate) + " " + Time.toString(typeTime) + " " + " " + "Будильник " + Info;

    ui->listWidget->addItem(outString);

}else{

    ui->timeEdit->setTime(QTime::currentTime());

    ui->textEdit->setText("");

    QMessageBox::warning(this,"Помилка вводу","Некоректні дані,перевірте ввід");
}




}


//add timer
void MainWindow::on_pushButton_2_clicked()
{
    int h = ui->lineEdit->text().toInt();

    int m = ui->lineEdit_2->text().toInt();

    int s = ui->lineEdit_3->text().toInt();



    if(( h <= 0 && m <= 0 && s <= 0 ) || h >= 24 || m >= 60 || s >= 60){

        QMessageBox::warning(this,"Помилка","Некоректні дані!");

        return void();
    }

    if(h <= 0 && m <= 0 && s <= 4){

    QMessageBox::warning(this,"Помилка","Мінімальний час таймера - 5 секунд!");

    return void();
    }

    QDate resD = QDate::currentDate();

    QTime resT = QTime::currentTime();

    int resSecond = resT.second();
    int resMinute = resT.minute();
    int resHour =  resT.hour();

    while(h != 0 || m != 0 || s != 0){

        if(resSecond + s >= 60){
            resSecond = resSecond + s - 60;
            resMinute = resMinute + 1;
            s = 0;
        }else{
            resSecond = resSecond + s;
            s = 0;
        }

        if(resMinute + m >= 60){
            resMinute = resMinute + m - 60;
            resHour = resHour + 1;
            m = 0;
        }else{
            resMinute = resMinute + m;
            m = 0;
        }

        if(resHour + h >= 24){
            resHour = resHour + h - 24;
            h = 0;
            resD = resD.addDays(1);
        }else{
            resHour = resHour + h;
            h = 0;
        }

        const QString fileway = ui->lineEdit_4->text();


     mainList.push_back(timeT(QTime(resHour,resMinute,resSecond),resD,fileway,"timer"));


      QString outString;
     if(fileway != "" || fileway != nullptr){

         QMessageBox::about(this,"Додано успішно",resD.toString(typeDate) + " " + QTime(resHour,resMinute,resSecond).toString(typeTime) + " "
                            + "Таймер \nЯкщо шлях вірний,то по закінченню часу відкриється файл: " + fileway);

         outString = resD.toString(typeDate) + " " + QTime(resHour,resMinute,resSecond).toString(typeTime) + " "
                 + "Таймер, відкриває файл: " + fileway;
     }else{
         QMessageBox::about(this,"Додано успішно",resD.toString(typeDate) + " " + QTime(resHour,resMinute,resSecond).toString(typeTime) + " " + "Таймер");
         outString = resD.toString(typeDate) + " " + QTime(resHour,resMinute,resSecond).toString(typeTime) + " " + "Таймер";

     }

    ui->lineEdit_4->setText("");
    ui->lineEdit->setText("0");
    ui->lineEdit_2->setText("0");
    ui->lineEdit_3->setText("0");


    ui->listWidget->addItem(outString);

    }
}


void MainWindow::on_radioButton_3_clicked()
{
    typeTime = "hh:mm:ss";
}


void MainWindow::on_radioButton_4_clicked()
{
    typeTime = "hh:mm";
}


void MainWindow::on_radioButton_5_clicked()
{
    typeTime = "HH.m.s";
}




