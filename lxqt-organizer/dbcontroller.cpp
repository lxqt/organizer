#include "dbcontroller.h"

DbController::DbController()
{

}

bool DbController::isOpen() const
{
    return db.isOpen();
}

void DbController::OpenDatabase()
{
    if(QSqlDatabase::isDriverAvailable("QSQLITE"))
           {
               db = QSqlDatabase::addDatabase("QSQLITE");

               db.setDatabaseName("eventsdb.sqlite");

               if(!db.open())
                   qWarning() << "Database connection error: " << db.lastError().text();
           }
           else
               qWarning() << ":Database connection error: no SQLite driver";

}

void DbController::CreateDatabaseTable()
{
    QSqlQuery query;
        query.prepare("CREATE TABLE appointment(ID INTEGER PRIMARY KEY, "
                      "Title TEXT, Location TEXT, Day INTEGER, Month INTEGER, Year INTEGER,"
                      "Starts INTEGER, Ends INTEGER, ReminderDay INTEGER, "
                      "ReminderMonth INTEGER, ReminderYear INTEGER, ReminderTime INTEGER);");

        if (!query.exec())
        {
            //qDebug() << "Table 'appointment' already exists or not created";
        }
        else {
           // qDebug() << "Table 'appointment' created";

        }
}

QSqlQuery DbController::SelectAllRecords()
{
    QSqlQuery query("SELECT * FROM appointment");
    return query;
}

QSqlQuery DbController::selectDateDescription()
{
    QSqlQuery query("SELECT ID, Title, Location FROM appointment");
    return query;
}

QSqlQuery DbController::selectDateTimeDescription()
{
    QSqlQuery query("SELECT ID, Title, Location, Day, Month, Year, Starts, Ends FROM appointment");
    return query;
}

QSqlQuery DbController::selectAppointmentById(int id)
{
    QSqlQuery result;
    result.prepare("SELECT * FROM appointment WHERE ID = (:idin)");
    result.bindValue(":idin", id);
    return result;
}

bool DbController::removeAllAppointments()
{
    bool success = false;
    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM appointment");
    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all appointments failed: " << removeQuery.lastError();
    }
    return success;
}

bool DbController::removeAppointmentById(const int id)
{
    bool success = false;
    //qDebug() << "Appointment id = "<<id;
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM appointment WHERE ID = (:idin)");
    queryDelete.bindValue(":idin", id);
    success = queryDelete.exec();
    if(!success)
    {
        qDebug() << "remove appointment failed: " << queryDelete.lastError();
    }
    //qDebug()<< "Remove appointment success = "<<success;
    return success;
}

void DbController::addAppointment(QString &title, QString &location,
                                  int &day, int &month, int &year,
                                  int &starttime, int &endtime,
                                  int &remday, int &remmonth, int &remyear,
                                  int &remtime)
{
    QSqlQuery query;

       query.prepare("INSERT INTO appointment(`ID`,`Title`,`Location`,`Day`,`Month`,`Year`,`Starts`,`Ends`,`ReminderDay`,`ReminderMonth`,`ReminderYear`,`ReminderTime`) "
                     "VALUES (:idin, :tin, :lin,:dyin, :mnin, :yrin, :stin,:etin,:drin, :mrin,:yrrin,:rtin);");
       //query.bindValue(":idin", 1); //autoincrement as primary key
       query.bindValue(":tin", title);
        query.bindValue(":lin",location);
       query.bindValue(":dyin", day);
       query.bindValue(":mnin", month);
       query.bindValue(":yrin", year);
       query.bindValue(":stin", starttime);
       query.bindValue(":etin", endtime);
       query.bindValue(":drin", remday);
       query.bindValue(":mrin", remmonth);
       query.bindValue(":yrrin", remyear);
       query.bindValue(":rtin", remtime);
       bool success = query.exec();
       if (success) {
           //qDebug() << "added new record";
       } else {
           qDebug() << "new record add failed" << query.lastError();
       }
}















