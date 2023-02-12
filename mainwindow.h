#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include<QCalendarWidget>

#include<QTime>
#include<QDate>

#include<QTimer>
#include<QVector>

#include<QMessageBox>

#include<fstream>

#include <Windows.h>

#include<plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"






using namespace std;

class timeT{
public:
    QTime time;
    QDate date;
    QString info;
    QString type;

    timeT(QTime time, QDate date, QString info, QString type){
        this->date = date;
        this->time = time;
        this->info = info;
        this->type = type;
    }

    timeT(){

    }

};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    QTimer *timer;

    QTime time;
    QDate date;

QString typeDate;
QString typeTime;
private slots:
    void TimerSlot();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
};
#endif // MAINWINDOW_H
