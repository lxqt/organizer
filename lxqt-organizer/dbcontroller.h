#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

class DbController
{
public:
    DbController();
    QSqlDatabase db;
    bool isOpen() const;
    void OpenDatabase();
    void CreateDatabaseTable();

    QSqlQuery SelectAllRecords();
    QSqlQuery selectDateDescription();
    QSqlQuery selectDateTimeDescription();
    QSqlQuery selectAppointmentById(int id);

    bool removeAllAppointments();
    bool removeAppointmentById(const int id);

    void addAppointment(QString &title, QString &location,
                        int &day, int &month, int &year,
                        int &starttime, int &endtime,
                        int &remday,int &remmonth, int &remyear,
                        int &remtime);




};



#endif // DBCONTROLLER_H
