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

        db.setDatabaseName("organizerdb.sqlite");
        if(!db.open())
            qWarning() << "Database connection error: " << db.lastError().text();
    }
    else
        qWarning() << ":Database connection error: no SQLite driver";
}


void DbManager::createDatebaseTables()
{
    QSqlQuery query;
   //QString sql1 = "CREATE TABLE IF NOT EXISTS appointments (AppointmentId INTEGER PRIMARY KEY,";

    QString sql1 = "CREATE TABLE appointments (AppointmentId INTEGER PRIMARY KEY,";
    sql1.append("Title TEXT,");
    sql1.append("Location TEXT,");
    sql1.append("Description TEXT,");
    sql1.append("StartTimestamp TEXT,");
    sql1.append("EndTimestamp TEXT,");
    sql1.append("ReminderRequest INTEGER,");
    sql1.append("ReminderTimestamp TEXT");
    sql1.append(");");

    QString sql2 = "CREATE TABLE contacts (ContactId INTEGER PRIMARY KEY,";
    sql2.append("FirstName TEXT,");
    sql2.append("LastName TEXT,");
    sql2.append("Email TEXT,");
    sql2.append("Street TEXT,");
    sql2.append("City TEXT,");
    sql2.append("County TEXT,");
    sql2.append("Postcode TEXT,");
    sql2.append("Country TEXT,");
    sql2.append("Telephone TEXT,");
    sql2.append("BirthdayTimestamp TEXT,");
    sql2.append("ReminderRequest INTEGER,");
    sql2.append("ReminderTimestamp TEXT");
    sql2.append(");");

    query.prepare(sql1);
    if(!query.exec())
    {
        //qDebug()<<"Table appointments already exists";
    }
    else {
        //qDebug()<<"Table appointments successfully created";
    }
    query.prepare(sql2);
    if(!query.exec())
    {
        //qDebug()<<"Table contacts already exists";
    }
    else {
        //qDebug()<<"Table contacts successfully created";
    }   
}

bool DbManager::removeAllAppointments()
{
    bool success = false;
    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM appointments");
    if (removeQuery.exec())
    {
        qDebug() << "remove all appointments succeeded";
        success = true;
    }
    else
    {
        qDebug() << "remove all appointments failed: " << removeQuery.lastError();
    }
    return success;
}

bool DbManager::removeAppointmentById(const int id)
{
    bool success = false;
    qDebug() << "Appointment id = "<<id;
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM appointments WHERE AppointmentId = (:idin)");
    queryDelete.bindValue(":idin", id);
    success = queryDelete.exec();
    if(!success)
    {
        qDebug() << "remove appointment failed: " << queryDelete.lastError();
    }
    qDebug()<< "Remove appointment success = "<<success;
    return success;
}

bool DbManager::addAppointment(Appointment &appointment)
{

    QString title=appointment.m_title;
    QString location=appointment.m_location;
    QString description =appointment.m_description;
    QString startTimestamp=appointment.m_appointmentStartTimestamp;
    QString endTimestamp=appointment.m_appointmentEndTimestamp;
    int reminderRequest=appointment.m_reminderRequest;
    QString reminderTimestamp=appointment.m_reminderTimestamp;

    QString sql="INSERT INTO appointments(`AppointmentId`,";
    sql.append("`Title`,");
    sql.append("`Location`,");
    sql.append("`Description`,");
    sql.append("`StartTimestamp`,");
    sql.append("`EndTimestamp`,");
    sql.append("`ReminderRequest`,");
    sql.append("`ReminderTimestamp`)");
    sql.append("VALUES (:idin,");
    sql.append(":titlein,");
    sql.append(":locationin,");
    sql.append(":descriptionin,");
    sql.append(":starttimestampin,");
    sql.append(":endtimestampin,");
    sql.append(":reminderrequestin,");
    sql.append(":remindertimestampin);");

    //qDebug()<<"sql query = "<<sql;

    QSqlQuery query;
    query.prepare(sql);

    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":titlein", title);
    query.bindValue(":locationin",location);
    query.bindValue(":descriptionin",description);
    query.bindValue(":starttimestampin",startTimestamp);
    query.bindValue(":endtimestampin",endTimestamp);
    query.bindValue(":reminderrequestin",reminderRequest);
    query.bindValue(":remindertimestampin",reminderTimestamp);

    bool success = query.exec();
    if (success) {
        //qDebug() << "added new record";
        return success;
    } else {
        //qDebug() << "new record add failed" << query.lastError();
        return success;
    }

}

bool DbManager::addContact(Contact &contact)
{
    QString firstName =contact.m_firstname;
    QString lastName =contact.m_lastname;
    QString email =contact.m_email;
    QString street =contact.m_street;
    QString city =contact.m_city;
    QString county =contact.m_county;
    QString postcode = contact.m_postcode;
    QString country =contact.m_country;
    QString telephone = contact.m_telephone;
    QString birthdayTimestamp=contact.m_birthdayTimestamp;
    int reminderRequest =contact.m_reminderRequest;
    QString reminderTimestamp =contact.m_reminderTimestamp;

    QString sql="INSERT INTO contacts(`ContactId`,";
    sql.append("`FirstName`,");
    sql.append("`LastName`,");
    sql.append("`Email`,");
    sql.append("`Street`,");
    sql.append("`City`,");
    sql.append("`County`,");
    sql.append("`Postcode`,");
    sql.append("`Country`,");
    sql.append("`Telephone`,");
    sql.append("`BirthdayTimestamp`,");
    sql.append("`ReminderRequest`,");
    sql.append("`ReminderTimestamp`)");
    sql.append("VALUES (:idin,");
    sql.append(":firstnamein,");
    sql.append(":lastnamein,");
    sql.append(":emailin,");
    sql.append(":streetin,");
    sql.append(":cityin,");
    sql.append(":countyin,");
    sql.append(":postcodein,");
    sql.append(":countryin,");
    sql.append(":telephonein,");
    sql.append(":birthdaytimestampin,");
    sql.append(":reminderrequestin,");
    sql.append(":remindertimestampin);");

    qDebug()<<"sql query = "<<sql;

    QSqlQuery query;
    query.prepare(sql);

    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":firstnamein", firstName);
    query.bindValue(":lastnamein", lastName);
    query.bindValue(":emailin", email);
    query.bindValue(":streetin", street);
    query.bindValue(":cityin", city);
    query.bindValue(":countyin", county);
    query.bindValue(":postcodein", postcode);
    query.bindValue(":countryin", country);
    query.bindValue(":telephonein", telephone);
    query.bindValue(":birthdaytimestampin", birthdayTimestamp);
    query.bindValue(":reminderrequestin", reminderRequest);
    query.bindValue(":remindertimestampin", reminderTimestamp);

    bool success = query.exec();
    if (success) {
        //qDebug() << "added new record";
        return success;
    } else {
        //qDebug() << "new record add failed" << query.lastError();
        return success;
    }
}

QList<Appointment> DbManager::getAllAppointments()
{
    QList<Appointment> appointmentList;
    //qDebug()<<"getting all appointments...";
    QSqlQuery query("SELECT * FROM appointments");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("AppointmentId");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("Title");
        QString title = query.value(idName).toString();

        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();

        idName = query.record().indexOf("Description");
        QString description = query.value(idName).toString();

        idName = query.record().indexOf("StartTimestamp");
        QString startTimestamp = query.value(idName).toString();

        idName = query.record().indexOf("EndTimestamp");
        QString endTimestamp = query.value(idName).toString();

        idName = query.record().indexOf("ReminderRequest");
        int reminderRequest = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTimestamp");
        QString reminderTimestamp = query.value(idName).toString();
        //Create a temporatory appointment
        Appointment tmp;
        tmp.m_id=id;
        tmp.m_title=title;
        tmp.m_location=location;
        tmp.m_description=description;
        tmp.m_appointmentStartTimestamp=startTimestamp;
        tmp.m_appointmentEndTimestamp=endTimestamp;
        tmp.m_reminderRequest=reminderRequest;
        tmp.m_reminderTimestamp=reminderTimestamp;
        appointmentList.append(tmp);
}
    return appointmentList;
}

QList<Contact> DbManager::getAllContacts()
{
    QList<Contact> contactList;
    //qDebug()<<"getting all contacts...";
    QSqlQuery query("SELECT * FROM contacts");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("ContactId");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("FirstName");
        QString firstname = query.value(idName).toString();

        idName = query.record().indexOf("LastName");
        QString lastname = query.value(idName).toString();

        idName = query.record().indexOf("Email");
        QString email = query.value(idName).toString();

        idName = query.record().indexOf("Street");
        QString street = query.value(idName).toString();

        idName = query.record().indexOf("City");
        QString city = query.value(idName).toString();

        idName = query.record().indexOf("County");
        QString county = query.value(idName).toString();

        idName = query.record().indexOf("Postcode");
        QString postcode = query.value(idName).toString();

        idName = query.record().indexOf("Country");
        QString country = query.value(idName).toString();

        idName = query.record().indexOf("Telephone");
        QString telephone = query.value(idName).toString();

        idName = query.record().indexOf("BirthdayTimestamp");
        QString birthdaytimestamp = query.value(idName).toString();

        idName = query.record().indexOf("ReminderRequest");
        int reminderrequest = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTimestamp");
        QString remindertimestamp = query.value(idName).toString();

        Contact tmp;
        tmp.m_id=id;
        tmp.m_firstname=firstname;
        tmp.m_lastname=lastname;
        tmp.m_email=email;
        tmp.m_street=street;
        tmp.m_city=city;
        tmp.m_county=county;
        tmp.m_postcode=postcode;
        tmp.m_country=country;
        tmp.m_telephone=telephone;
        tmp.m_birthdayTimestamp=birthdaytimestamp;
        tmp.m_reminderRequest=reminderrequest;
        tmp.m_reminderTimestamp=remindertimestamp;
        contactList.append(tmp);
    }
    return contactList;
}

bool DbManager::removeAllContacts()
{
    bool success = false;
    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM contacts");
    if (removeQuery.exec())
    {
        //qDebug() << "remove all contacts succeeded";
        success = true;
    }
    else
    {
        //qDebug() << "remove all contacts failed: " << removeQuery.lastError();
    }
    return success;

}

bool DbManager::removeContactById(const int id)
{
    bool success = false;
    qDebug() << "Contact id = "<<id;
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM contacts WHERE ContactId = (:idin)");
    queryDelete.bindValue(":idin", id);
    success = queryDelete.exec();
    if(!success)
    {
        //qDebug() << "remove contact failed: " << queryDelete.lastError();
    }
   // qDebug()<< "Remove contact success = "<<success;
    return success;
}

QList<Contact> DbManager::findContactsByLastName(QString lastName)
{
    //Could be contacts with the same lastname so need to create a list
    QList<Contact> contactsList = QList<Contact>();
    //query.bindValue(":lname",lastName);
    //qDebug() << query.boundValue(":lname");

    qDebug()<<"lastname input = "<<lastName;
    QSqlQuery query;
    QString sql ="SELECT * FROM contacts WHERE LastName ='"+lastName+"'";
    qDebug()<<"Search query = "<<sql;
    query.prepare(sql);
    query.exec(sql);

    while (query.next())
    {
        qDebug()<<"entering query.next()";
        int idName = query.record().indexOf("ContactId");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("FirstName");
        QString firstname = query.value(idName).toString();

        idName = query.record().indexOf("LastName");
        QString lastname = query.value(idName).toString();

        idName = query.record().indexOf("Email");
        QString email = query.value(idName).toString();

        idName = query.record().indexOf("Street");
        QString street = query.value(idName).toString();

        idName = query.record().indexOf("City");
        QString city = query.value(idName).toString();

        idName = query.record().indexOf("County");
        QString county = query.value(idName).toString();

        idName = query.record().indexOf("Postcode");
        QString postcode = query.value(idName).toString();

        idName = query.record().indexOf("Country");
        QString country = query.value(idName).toString();

        idName = query.record().indexOf("Telephone");
        QString telephone = query.value(idName).toString();

        idName = query.record().indexOf("BirthdayTimestamp");
        QString birthdaytimestamp = query.value(idName).toString();

        idName = query.record().indexOf("ReminderRequest");
        int reminderrequest = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTimestamp");
        QString remindertimestamp = query.value(idName).toString();

        Contact tmp;
        tmp.m_id=id;
        tmp.m_firstname=firstname;
        tmp.m_lastname=lastname;
        tmp.m_email=email;
        tmp.m_street=street;
        tmp.m_city=city;
        tmp.m_county=county;
        tmp.m_postcode=postcode;
        tmp.m_country=country;
        tmp.m_telephone=telephone;
        tmp.m_birthdayTimestamp=birthdaytimestamp;
        tmp.m_reminderRequest=reminderrequest;
        tmp.m_reminderTimestamp=remindertimestamp;
        contactsList.append(tmp);
    }
    return contactsList;
}

QList<Appointment> DbManager::getAppointmentsToday()
{

    //Construct an empty list
    //    // QList<Appointment> appointmentList;

    QList<Appointment> appointmentList = QList<Appointment>();
    QDate currentDate= QDate::currentDate();

    QSqlQuery query("SELECT * FROM appointments");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("AppointmentId");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("Title");
        QString title = query.value(idName).toString();

        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();

        idName = query.record().indexOf("Description");
        QString description = query.value(idName).toString();

        idName = query.record().indexOf("StartTimestamp");
        QString startTimestamp = query.value(idName).toString();

        idName = query.record().indexOf("EndTimestamp");
        QString endTimestamp = query.value(idName).toString();

        idName = query.record().indexOf("ReminderRequest");
        int reminderRequest = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTimestamp");
        QString reminderTimestamp = query.value(idName).toString();

        //Need to get the date from the start timestamp
        QDateTime startdatetime =QDateTime::fromString(startTimestamp);
        QDate eventDate =startdatetime.date();

        if(eventDate ==currentDate)
        {
            Appointment tmp;
            tmp.m_id=id;
            tmp.m_title=title;
            tmp.m_location=location;
            tmp.m_description=description;
            tmp.m_appointmentStartTimestamp=startTimestamp;
            tmp.m_appointmentEndTimestamp=endTimestamp;
            tmp.m_reminderRequest=reminderRequest;
            tmp.m_reminderTimestamp=reminderTimestamp;
            appointmentList.append(tmp);
        }
    }
    return appointmentList;
}

QList<Appointment> DbManager::getAppointmentsOnDate(QDate &diaryDate)
{
    QDate checkDate = QDate(diaryDate);

    QList<Appointment> appointmentList = QList<Appointment>(); //Construct an empty list
    QSqlQuery query("SELECT * FROM appointments");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("AppointmentId");
        int id = query.value(idName).toInt();

        idName = query.record().indexOf("Title");
        QString title = query.value(idName).toString();

        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();

        idName = query.record().indexOf("Description");
        QString description = query.value(idName).toString();

        idName = query.record().indexOf("StartTimestamp");
        QString startTimestamp = query.value(idName).toString();

        idName = query.record().indexOf("EndTimestamp");
        QString endTimestamp = query.value(idName).toString();

        idName = query.record().indexOf("ReminderRequest");
        int reminderRequest = query.value(idName).toInt();

        idName = query.record().indexOf("ReminderTimestamp");
        QString reminderTimestamp = query.value(idName).toString();

        //Need to get the date from the start timestamp
        QDateTime startdatetime =QDateTime::fromString(startTimestamp);


        QDate eventDate =startdatetime.date();

        if(eventDate ==checkDate)
        {
            Appointment tmp;
            tmp.m_id=id;
            tmp.m_title=title;
            tmp.m_location=location;
            tmp.m_description=description;
            tmp.m_appointmentStartTimestamp=startTimestamp;
            tmp.m_appointmentEndTimestamp=endTimestamp;
            tmp.m_reminderRequest=reminderRequest;
            tmp.m_reminderTimestamp=reminderTimestamp;
            appointmentList.append(tmp);
        }
    }
    return appointmentList;
}
