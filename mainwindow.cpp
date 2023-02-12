#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;



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

    plog::init(plog::verbose,"C:\\Users\\user\\Documents\\GitHub\\OOP2\\plogBase.txt");

    PLOG_VERBOSE << "PROGRAM OPENED";

    /*
    PLOG_VERBOSE << "verbose";
    PLOG_DEBUG << "debug";
    PLOG_INFO << "info";
    PLOG_WARNING << "warning";
    PLOG_ERROR << "error";
    PLOG_FATAL << "fatal";
    PLOG_NONE << "none";
    */

}

MainWindow::~MainWindow()
{
    PLOG_VERBOSE << "PROGRAM CLOSED";
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
    PLOG_DEBUG << "time and date = timeT";

        //2 variants timer and alarm
        if(child.type == "budilnik"){

            PLOG_DEBUG << "timeT budilnik time";
            if(child.info != ""){

              PLOG_DEBUG << "alarm budilnik with info,QMessageBox";

              MessageBeep(MB_ICONEXCLAMATION);
              QMessageBox::about(this,"Спрацював будильник!",child.info);


            }else{

                PLOG_DEBUG << "alarm budilnik without info,QMessageBox";
                MessageBeep(MB_ICONEXCLAMATION);
                QMessageBox::about(this,"Спрацював будильник!","Спрацював будильник без опису");

            }
        }else if(child.type == "timer"){

            PLOG_DEBUG << "timeT timer time";
            if(child.info != ""){

              PLOG_DEBUG << "alarm timer with info,QMessageBox,tried to open file " + child.info;
              MessageBeep(MB_ICONEXCLAMATION);

              const char* STRopen = (child.info.toStdString()).c_str();

              system(STRopen);

              QMessageBox::about(this,"Спрацював таймер!","Відкрито файл: " + child.info);



            }else{

              PLOG_DEBUG << "alarm timer with info,QMessageBox";
              MessageBeep(MB_ICONEXCLAMATION);
              QMessageBox::about(this,"Спрацював таймер!","Час вичерпано!");


            }

        }else{
            PLOG_WARNING << "error type";
            QMessageBox::warning(this,"Помилка типу","Помилка типу");

        }


        PLOG_DEBUG << "erase timeT with this time";
        mainList.erase(mainList.begin() + pos);


        ui->listWidget->clear();

        PLOG_DEBUG << "rewrite listWidget";

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




//add budilik
void MainWindow::on_pushButton_clicked()
{
PLOG_INFO << "pressed button <add budilnik>";

QDate Date = ui->calendarWidget->selectedDate();

QTime Time = ui->timeEdit->time();

Time.setHMS(Time.hour(),Time.minute(),0,0);

QString Info = ui->textEdit->toPlainText();

QString Type = "budilnik";


if(( Date == QDate::currentDate() && Time > QTime::currentTime() ) || ( Date > QDate::currentDate()) ){

    for(const timeT &child: mainList){

        if(child.date == Date && child.time == Time){

            PLOG_ERROR << "Budilnik with same time alredy exist";

            QMessageBox::warning(this,"Помилка","Будильник з таким часом вже є");

            return void();

        }


    }

    PLOG_DEBUG << "created class timeT, added to list mainList";
    timeT newNode(Time,Date,Info,Type);

    mainList.push_back(newNode);



    PLOG_NONE << "QMessageBox that budilnik added";
    QMessageBox::about(this,"Додано успішно",Date.toString(typeDate) + " " + Time.toString(typeTime) + " " + Info + " " + "Будильник");


    PLOG_NONE << "widget timeEdit set time curr, info line set void";
    ui->timeEdit->setTime(QTime::currentTime());
    ui->textEdit->setText("");



    PLOG_NONE << "add budilnik string to listWidget";
    QString outString = Date.toString(typeDate) + " " + Time.toString(typeTime) + " " + " " + "Будильник " + Info;

    ui->listWidget->addItem(outString);

}else{

    PLOG_ERROR << "wrong enter,QMessageBox";
    PLOG_NONE << "widget timeEdit set time curr, info line set void";

    ui->timeEdit->setTime(QTime::currentTime());

    ui->textEdit->setText("");

    QMessageBox::warning(this,"Помилка вводу","Некоректні дані,перевірте ввід");
}




}


//add timer
void MainWindow::on_pushButton_2_clicked()
{
    PLOG_INFO << "pressed button <add timer>";

    int h = ui->lineEdit->text().toInt();

    int m = ui->lineEdit_2->text().toInt();

    int s = ui->lineEdit_3->text().toInt();



    if(( h <= 0 && m <= 0 && s <= 0 ) || h >= 24 || m >= 60 || s >= 60){

        PLOG_ERROR << "wrong enter, time = 0 or more then can be";

        QMessageBox::warning(this,"Помилка","Некоректні дані!");

        return void();
    }

    if(h <= 0 && m <= 0 && s <= 4){

    PLOG_ERROR << "wrong enter, time less then 4 seconds";
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

        PLOG_DEBUG << "changed sec,m,h to good format";

        const QString fileway = ui->lineEdit_4->text();


        PLOG_DEBUG << "created class timeT, added to list mainList";
     mainList.push_back(timeT(QTime(resHour,resMinute,resSecond),resD,fileway,"timer"));


     PLOG_NONE << "add timer string to listWidget 2 variants - with path to open or without";
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

    ui->listWidget->addItem(outString);

    PLOG_NONE << "set timer widgets to basic";
    ui->lineEdit_4->setText("");
    ui->lineEdit->setText("0");
    ui->lineEdit_2->setText("0");
    ui->lineEdit_3->setText("0");




    }
}


void MainWindow::on_radioButton_3_clicked()
{
    PLOG_INFO << "Pressed radiobutton hh:mm:ss";
    typeTime = "hh:mm:ss";
}


void MainWindow::on_radioButton_4_clicked()
{
    PLOG_INFO << "Pressed radiobutton hh:mm";
    typeTime = "hh:mm";
}


void MainWindow::on_radioButton_5_clicked()
{
    PLOG_INFO << "Pressed radiobutton HH.m.s";
    typeTime = "HH.m.s";
}




