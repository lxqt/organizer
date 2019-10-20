#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>
#include "dbcontroller.h"
#include "dialogabout.h"
#include "simpledbusnotifier.h"


struct Date
{
    int d, m, y;
};

const int monthDays[12] = {31, 28, 31, 30, 31, 30,
                        31, 31, 30, 31, 30, 31};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DbController dbc;
    QDate eventDate;
    QStringListModel *model; //use model-view architecture
    int day=0;
    int month=0;
    int year=0;
    int startTime=0;
    int endTime=0;
    int currentID=0;
    QDate reminderDate;
    int remDay=0;
    int remMonth=0;
    int remYear=0;
    int remTime=0;
    int reminderDays=0;
    QString title="";
    QString location="";
    void InitialiseDates();
    void DisplayAllAppointments();
    int DayDifferenceBetweenDates(Date dt1, Date dt2); //Will rewrite with QDates
    int LeapYearNumber(Date d); //Will rewrite with QDate
    void CheckNotificationsForToday();
    void CheckNotificationReminders();



private slots:
    void on_pushButtonAddAppointment_clicked();

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_calendarWidget_clicked(const QDate &date);

    void on_actionDelete_All_triggered();

    void on_pushButtonClear_clicked();

    void on_pushButtonShowAll_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_pushButtonAppointmentToday_clicked();

    void on_pushButtonDelete_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
