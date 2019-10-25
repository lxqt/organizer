/***************************************************************************
 *   Author aka. crispina                 *
 *   crispinalan@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "dbmanager.h"

DbManager::DbManager()
{

}

bool DbManager::isOpen() const
{
    return db.isOpen();
}

void DbManager::openDatabase()
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

void DbManager::createDatabaseTable()
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

bool DbManager::removeAllAppointments()
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
       // qDebug() << "remove all appointments failed: " << removeQuery.lastError();
    }
    return success;
}

bool DbManager::removeAppointmentById(const int id)
{
    bool success = false;
    qDebug() << "Appointment id = "<<id;
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM appointment WHERE ID = (:idin)");
    queryDelete.bindValue(":idin", id);
    success = queryDelete.exec();
    if(!success)
    {
       // qDebug() << "remove appointment failed: " << queryDelete.lastError();
    }
   // qDebug()<< "Remove appointment success = "<<success;
    return success;
}

void DbManager::addAppointment(Event &appointment)
{
    QString title=appointment.title;
    QString location=appointment.location;
    int day =appointment.day;
    int month=appointment.month;
    int year=appointment.year;
    int starttime=appointment.startTime;
    int endtime=appointment.endTime;
    int remday=appointment.reminderDay;
    int remmonth=appointment.reminderMonth;
    int remyear=appointment.reminderYear;
    int remtime=appointment.reminderTime;

    QSqlQuery query;
    query.prepare("INSERT INTO appointment(`ID`,`Title`,`Location`,`Day`,`Month`,`Year`,`Starts`,`Ends`,`ReminderDay`,`ReminderMonth`,`ReminderYear`,`ReminderTime`) "
                  "VALUES (:idin, :tin, :lin,:dyin, :mnin, :yrin, :stin,:etin,:drin, :mrin,:yrrin,:rtin);");
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":tin", title); //title in
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
        qDebug() << "added new record";
    } else {
        qDebug() << "new record add failed" << query.lastError();
    }
}

void DbManager::addAppointment(QString &title, QString &location, int &day, int &month, int &year, int &starttime, int &endtime, int &remday, int &remmonth, int &remyear, int &remtime)
{
    QSqlQuery query;

          query.prepare("INSERT INTO appointment(`ID`,`Title`,`Location`,`Day`,`Month`,`Year`,`Starts`,`Ends`,`ReminderDay`,`ReminderMonth`,`ReminderYear`,`ReminderTime`) "
                        "VALUES (:idin, :tin, :lin,:dyin, :mnin, :yrin, :stin,:etin,:drin, :mrin,:yrrin,:rtin);");
          //query.bindValue(":idin", 1); //autoincrement as primary key
          query.bindValue(":tin", title); //title in
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
              qDebug() << "added new record";
          } else {
              qDebug() << "new record add failed" << query.lastError();
          }
}

QList<Event> DbManager::getAllAppointments()
{
    QList<Event> appointmentList;

    QSqlQuery query("SELECT * FROM appointment");

    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("ID");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("Title");
        QString title = query.value(idName).toString();

        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();

        idName = query.record().indexOf("Day");
        int day = query.value(idName).toInt();

        idName = query.record().indexOf("Month");
        int month = query.value(idName).toInt();

        idName = query.record().indexOf("Year");
        int year = query.value(idName).toInt();

        idName = query.record().indexOf("Starts");
        int starttime = query.value(idName).toInt();

        idName = query.record().indexOf("Ends");
        int endtime = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderDay");
        int remday = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderMonth");
        int remmonth = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderYear");
        int remyear = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTime");
        int remtime = query.value(idName).toInt();        

        //Create a temporatory appointment
        Event *a = new Event(id,
                                         title,
                                         location,
                                         day,
                                         month,
                                         year,
                                         starttime,
                                         endtime,
                                         remday,
                                         remmonth,
                                         remyear,
                                         remtime);

        appointmentList.append(*a);
       }
    return appointmentList;
}

QList<Event> DbManager::getAppointmentsToday()
{
    QDate currentDate = QDate::currentDate();
    QList<Event> appointmentList = QList<Event>(); //Construct an empty list
    QSqlQuery query("SELECT * FROM appointment");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("ID");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("Title");
        QString title = query.value(idName).toString();

        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();

        idName = query.record().indexOf("Day");
        int day = query.value(idName).toInt();

        idName = query.record().indexOf("Month");
        int month = query.value(idName).toInt();

        idName = query.record().indexOf("Year");
        int year = query.value(idName).toInt();

        idName = query.record().indexOf("Starts");
        int starttime = query.value(idName).toInt();

        idName = query.record().indexOf("Ends");
        int endtime = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderDay");
        int remday = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderMonth");
        int remmonth = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderYear");
        int remyear = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTime");
        int remtime = query.value(idName).toInt();

        QDate eventDate =QDate(year,month,day);

        if (eventDate==currentDate){

            Event *a = new Event(id,
                                             title,
                                             location,
                                             day,
                                             month,
                                             year,
                                             starttime,
                                             endtime,
                                             remday,
                                             remmonth,
                                             remyear,
                                             remtime);

            appointmentList.append(*a);
        }
       }
    return appointmentList;
}

QList<Event> DbManager::getAppointmentsOnDate(QDate *diaryDate)
{
    QDate checkDate = QDate(*diaryDate);


    QList<Event> appointmentList = QList<Event>(); //Construct an empty list
    QSqlQuery query("SELECT * FROM appointment");

    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("ID");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("Title");
        QString title = query.value(idName).toString();

        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();

        idName = query.record().indexOf("Day");
        int day = query.value(idName).toInt();

        idName = query.record().indexOf("Month");
        int month = query.value(idName).toInt();

        idName = query.record().indexOf("Year");
        int year = query.value(idName).toInt();

        idName = query.record().indexOf("Starts");
        int starttime = query.value(idName).toInt();

        idName = query.record().indexOf("Ends");
        int endtime = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderDay");
        int remday = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderMonth");
        int remmonth = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderYear");
        int remyear = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTime");
        int remtime = query.value(idName).toInt();

        QDate eventDate =QDate(year,month,day);

        if (eventDate==checkDate){

            Event *a = new Event(id,
                                             title,
                                             location,
                                             day,
                                             month,
                                             year,
                                             starttime,
                                             endtime,
                                             remday,
                                             remmonth,
                                             remyear,
                                             remtime);

            appointmentList.append(*a);
        }
       }
    return appointmentList;
}
