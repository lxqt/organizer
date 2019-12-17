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
    sql1.append("Date TEXT,");
    sql1.append("StartTime TEXT,");
    sql1.append("EndTime TEXT,");
    sql1.append("Category TEXT,");
    sql1.append("IsFullDay INTEGER,");
    sql1.append("IsRepeating INTEGER,");
    sql1.append("AddBirthday INTEGER,");
    sql1.append("ParentId INTEGER,");
    sql1.append("HasReminder INTEGER");
    sql1.append(");");    

    QString sql2 = "CREATE TABLE reminders (AppointmentId INTEGER PRIMARY KEY,";
    sql2.append("Details TEXT,");
    sql2.append("ReminderDate TEXT,");
    sql2.append("ReminderTime TEXT");
    sql2.append(");");

    QString sql3 = "CREATE TABLE contacts (ContactId INTEGER PRIMARY KEY,";
    sql3.append("FirstName TEXT,");
    sql3.append("MidName TEXT,");
    sql3.append("LastName TEXT,");
    sql3.append("Email TEXT,");
    sql3.append("Street TEXT,");
    sql3.append("District TEXT,");
    sql3.append("City TEXT,");
    sql3.append("County TEXT,");
    sql3.append("Postcode TEXT,");
    sql3.append("Country TEXT,");
    sql3.append("Telephone TEXT,");
    sql3.append("BirthDate TEXT,");
    sql3.append("AddBirthday INTEGER,");
    sql3.append("AppointmentId INTEGER");
    sql3.append(");");

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
       //qDebug()<<"Table reminders already exists";
    }
    else {
       //qDebug()<<"Table reminders successfully created";
    }

    query.prepare(sql3);
    if(!query.exec())
    {
        //qDebug()<<"Table contacts already exists";
    }
    else {
        //qDebug()<<"Table contacts successfully created";
    }

}

//------------------------------------------------------------------
//Reminders
//------------------------------------------------------------------

bool DbManager::addReminder(Reminder &reminder)
{
    int appointmentId =reminder.m_appointmentId;
    QString details =reminder.m_details;
    QString reminderdate =reminder.m_reminderDate;
    QString remindertime =reminder.m_reminderTime;

    QString sql="INSERT INTO reminders(`AppointmentId`,";
    sql.append("`Details`,");
    sql.append("`ReminderDate`,");
    sql.append("`ReminderTime`)");
    sql.append("VALUES (:appointmentidin,");
    sql.append(":detailsin,");
    sql.append(":reminderdatein,");
    sql.append(":remindertimein);");

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":appointmentidin", appointmentId);
    query.bindValue(":detailsin", details);
    query.bindValue(":reminderdatein",reminderdate);
    query.bindValue(":remindertimein",remindertime);
    query.exec();
    int id = query.lastInsertId().toInt();
    return id;
}

bool DbManager::updateReminder(Reminder &reminder, int id)
{
    //qDebug()<<"Entering update reminder....";
    bool success =false;
    QString sql ="UPDATE reminders SET ";
    sql.append(" Details = :din");
    sql.append(", ReminderDate = :rdin");
    sql.append(", ReminderTime = :rtin");
    sql.append(" WHERE AppointmentId =:idin");
    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(":din", reminder.m_details);
      qry.bindValue(":rdin", reminder.m_reminderDate);
      qry.bindValue(":rtin", reminder.m_reminderTime);
      qry.bindValue(":idin", QString::number(id));
      success=qry.exec();
      if (success)
      {
        //qDebug() << "Reminder updated" << " success ="<<success;
        return success;  //true
      }
      else
      {
        //qDebug() << "Reminder update failed"<<" sucess ="<<success;
        return success;
      }
    }
    else
    {
      //qDebug() << "SQL is broken"<<" success ="<<success;
      return success;
    }
}

QList<Reminder> DbManager::getAllReminders()
{
    QList<Reminder> reminderList;
    QSqlQuery query("SELECT * FROM reminders");
    query.exec();

    while (query.next())
    {
        //int idName = query.record().indexOf("ReminderId");
        //int id = query.value(idName).toInt();
        int idName = query.record().indexOf("AppointmentId");
        int aid = query.value(idName).toInt();
        idName = query.record().indexOf("Details");
        QString details = query.value(idName).toString();
        idName = query.record().indexOf("ReminderDate");
        QString reminderDate = query.value(idName).toString();
        idName = query.record().indexOf("ReminderTime");
        QString reminderTime = query.value(idName).toString();
        idName = query.record().indexOf("ReminderRequest");
        //int reminderRequest = query.value(idName).toInt();
        //Create a temporary reminder
        Reminder r;
        //r.m_id=id;
        r.m_appointmentId=aid;
        r.m_details=details;
        r.m_reminderDate=reminderDate;
        r.m_reminderTime= reminderTime;
        //r.m_reminderRequest=reminderRequest;
        reminderList.append(r);
    }
    return reminderList;
}


Reminder DbManager::getReminderByID(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM reminders WHERE ReminderId = (:idin)");
    query.bindValue(":idin", id);
    query.exec();
    query.first();
    //int idName = query.record().indexOf("ReminderId");
    //int idd = query.value(idName).toInt();
    int idName = query.record().indexOf("AppointmentId");
    int aid = query.value(idName).toInt();
    idName = query.record().indexOf("Details");
    QString details = query.value(idName).toString();
    idName = query.record().indexOf("ReminderDate");
    QString reminderdate = query.value(idName).toString();
    idName = query.record().indexOf("ReminderTime");
    QString remindertime = query.value(idName).toString();
    idName = query.record().indexOf("ReminderRequest");
    //int reminderrequest = query.value(idName).toInt();
    //Create a reminder and return it
    Reminder r;
    //r.m_id=idd;
    r.m_appointmentId=aid;
    r.m_details=details;
    r.m_reminderDate=reminderdate;
    r.m_reminderTime=remindertime;
   // r.m_reminderRequest=reminderrequest;
    return r;
}


bool DbManager::removeReminderById(const int appointmentId)
{
    bool success = false;
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM reminders WHERE AppointmentId = (:idin)");
    queryDelete.bindValue(":idin", appointmentId);
    success = queryDelete.exec();
    if(!success)
    {
        //qDebug() << "remove reminder failed: " << queryDelete.lastError();
    }
    //qDebug()<< "Remove reminder success = "<<success;
    return success;
}

bool DbManager::removeAllReminders()
{
    bool success = false;
    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM reminders");
    if (removeQuery.exec())
    {
        //qDebug() << "remove all reminders succeeded";
        success = true;
    }
    else
    {
        //qDebug() << "remove all reminders failed: " << removeQuery.lastError();
    }
    return success;
}




//-------------------------------------------------------------
// Appointments
//--------------------------------------------------------------
bool DbManager::deleteAllAppointments()
{
    bool success = false;
    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM appointments");
    if (removeQuery.exec())
    {
        //qDebug() << "remove all appointments succeeded";
        success = true;
    }
    else
    {
        //qDebug() << "remove all appointments failed: " << removeQuery.lastError();
    }
    return success;
}



bool DbManager::deleteAppointmentById(const int id)
{
    bool success = false;
    //qDebug() << "Appointment id = "<<id;
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM appointments WHERE AppointmentId = (:idin)");
    queryDelete.bindValue(":idin", id);
    success = queryDelete.exec();
    if(!success)
    {
       //qDebug() << "remove appointment failed: " << queryDelete.lastError();
    }
    //qDebug()<< "Remove appointment success = "<<success;
    return success;
}

bool DbManager::deleteAppointmentByParentID(const int parentId)
{
    bool success = false;
    qDebug() << "Parent id = "<<parentId;
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM appointments WHERE ParentId = (:idin)");
    queryDelete.bindValue(":idin", parentId);
    success = queryDelete.exec();
    if(!success)
    {
       //qDebug() << "delete appointment by parentID failed: " << queryDelete.lastError();
    }
    //qDebug()<< "Deleted appointment by parentID success = "<<success;
    return success;

}

int DbManager::addAppointment(Appointment &appointment)
{
    QString title=appointment.m_title;
    QString location=appointment.m_location;
    QString description =appointment.m_description;
    QString date = appointment.m_date;
    QString startTime=appointment.m_startTime;
    QString endTime=appointment.m_endTime;
    QString category=appointment.m_category;
    int isFullDay=appointment.m_isFullDay;
    int isRepeating =appointment.m_isRepeating;
    int addBirthday =appointment.m_addBirthday;
    int parentId=appointment.m_parentId;
    int hasreminder =appointment.m_hasReminder;


    QString sql="INSERT INTO appointments(`AppointmentId`,";
    sql.append("`Title`,");
    sql.append("`Location`,");
    sql.append("`Description`,");
    sql.append("`Date`,");
    sql.append("`StartTime`,");
    sql.append("`EndTime`,");
    sql.append("`Category`,");
    sql.append("`IsFullDay`,");
    sql.append("`IsRepeating`,");
    sql.append("`AddBirthday`,");
    sql.append("`ParentId`,");
    sql.append("`HasReminder`)");

    sql.append("VALUES (:idin,");
    sql.append(":titlein,");
    sql.append(":locationin,");
    sql.append(":descriptionin,");
    sql.append(":datein,");
    sql.append(":starttimein,");
    sql.append(":endtimein,");
    sql.append(":catin,");
    sql.append(":isfulldayin,");
    sql.append(":isrepeatingin,");
    sql.append(":addbirthdayin,");
    sql.append(":parentidin,");
    sql.append(":hasreminderin);");


    QSqlQuery query;
    query.prepare(sql);
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":titlein", title);
    query.bindValue(":locationin",location);
    query.bindValue(":descriptionin",description);
    query.bindValue(":datein",date);
    query.bindValue(":starttimein",startTime);
    query.bindValue(":endtimein",endTime);
    query.bindValue(":catin",category);
    query.bindValue(":isfulldayin",isFullDay);
    query.bindValue(":isrepeatingin",isRepeating);
    query.bindValue(":addbirthdayin",addBirthday);
    query.bindValue(":parentidin",parentId);
    query.bindValue(":hasreminderin",hasreminder);


    query.exec();
    int id = query.lastInsertId().toInt();
    //qDebug() << "added new appointment with ID = "<<id;
    return id;
}

bool DbManager::updateAppointment(Appointment &appointment, int id)
{
    bool success =false;    

    QString sql ="UPDATE appointments SET ";
    sql.append(" Title = :tin");
    sql.append(", Location = :lin");
    sql.append(", Description = :din");
    sql.append(", Date = :datein");
    sql.append(", StartTime = :stin");
    sql.append(", EndTime = :etin");
    sql.append(", Category = :catin");
    sql.append(", IsFullDay = :isfdin");
    sql.append(", IsRepeating = :isrin");
    sql.append(", AddBirthday = :addbin");
    sql.append(", ParentId = :pidin");
    sql.append(", HasReminder = :hrin");    
    sql.append(" WHERE AppointmentId =:idin");

    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(":tin", appointment.m_title);
      qry.bindValue(":lin", appointment.m_location);
      qry.bindValue(":din", appointment.m_description);
      qry.bindValue(":datein", appointment.m_date);
      qry.bindValue(":stin",appointment.m_startTime);
      qry.bindValue(":etin",appointment.m_endTime);
      qry.bindValue(":catin",appointment.m_category);
      qry.bindValue(":isfdin",appointment.m_isFullDay);
      qry.bindValue(":isrin",appointment.m_isRepeating);
      qry.bindValue(":addbin",appointment.m_addBirthday);
      qry.bindValue(":pidin",appointment.m_parentId);
      qry.bindValue(":hrin",appointment.m_hasReminder);

      qry.bindValue(":idin", QString::number(id));
      success=qry.exec();
      if (success)
      {
        qDebug() << "Appointment updated" << " sucess ="<<success;
        return success;  //true
      }
      else
      {
        qDebug() << "Appointment update failed"<<" sucess ="<<success;
        return success;
      }
    }
    else
    {
      qDebug() << "SQL is broken"<<" sucess ="<<success;
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
        int aid = query.value(idName).toInt();
        idName = query.record().indexOf("Title");
        QString title = query.value(idName).toString();
        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();
        idName = query.record().indexOf("Description");
        QString description = query.value(idName).toString();
        idName = query.record().indexOf("Date");
        QString date = query.value(idName).toString();
        idName = query.record().indexOf("StartTime");
        QString startTime = query.value(idName).toString();
        idName = query.record().indexOf("EndTime");
        QString endTime = query.value(idName).toString();
        idName = query.record().indexOf("Category");
        QString category = query.value(idName).toString();

        idName = query.record().indexOf("IsFullDay");
        int isFullDay = query.value(idName).toInt();

        idName = query.record().indexOf("IsRepeating");
        int isRepeating = query.value(idName).toInt();

        idName = query.record().indexOf("AddBirthday");
        int addBirthday = query.value(idName).toInt();


        idName = query.record().indexOf("ParentId");
        int parentId = query.value(idName).toInt();

        idName = query.record().indexOf("HasReminder");
        int hasReminder = query.value(idName).toInt();
        Appointment tmp;
        tmp.m_id=aid;
        tmp.m_title=title;
        tmp.m_location=location;
        tmp.m_description=description;
        tmp.m_date=date;
        tmp.m_startTime=startTime;
        tmp.m_endTime=endTime;
        tmp.m_category=category;
        tmp.m_isFullDay=isFullDay;
        tmp.m_isRepeating=isRepeating;
        tmp.m_addBirthday=addBirthday;
        tmp.m_parentId=parentId;
        tmp.m_hasReminder=hasReminder;

        appointmentList.append(tmp);
}
    return appointmentList;
}

Appointment DbManager::getAppointmentByID(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM appointments WHERE AppointmentId = (:idin)");
    query.bindValue(":idin", id);
    query.exec();
    query.first();

    int idName = query.record().indexOf("AppointmentId");
    int idd = query.value(idName).toInt();

    idName = query.record().indexOf("Title");
    QString title = query.value(idName).toString();
    idName = query.record().indexOf("Location");
    QString location = query.value(idName).toString();
    idName = query.record().indexOf("Description");
    QString description = query.value(idName).toString();
    idName = query.record().indexOf("Date");
    QString date = query.value(idName).toString();
    idName = query.record().indexOf("StartTime");
    QString startTime = query.value(idName).toString();
    idName = query.record().indexOf("EndTime");
    QString endTime = query.value(idName).toString();
    idName = query.record().indexOf("Category");
    QString category = query.value(idName).toString();

    idName = query.record().indexOf("IsFullDay");
    int isFullDay = query.value(idName).toInt();

    idName = query.record().indexOf("IsRepeating");
    int isRepeating = query.value(idName).toInt();

    idName = query.record().indexOf("AddBirthday");
    int addBirthday = query.value(idName).toInt();


    idName = query.record().indexOf("ParentId");
    int parentId = query.value(idName).toInt();

    idName = query.record().indexOf("HasReminder");
    int hasReminder = query.value(idName).toInt();


    Appointment appointment;
    appointment.m_id=idd;
    appointment.m_title=title;
    appointment.m_location=location;
    appointment.m_description=description;
    appointment.m_date=date;
    appointment.m_startTime=startTime;
    appointment.m_endTime=endTime;
    appointment.m_category=category;
    appointment.m_isFullDay=isFullDay;
    appointment.m_isRepeating=isRepeating;
    appointment.m_addBirthday=addBirthday;
    appointment.m_parentId=parentId;
    appointment.m_hasReminder=hasReminder;


    return appointment;

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
        idName = query.record().indexOf("Date");
        QString date = query.value(idName).toString();
        idName = query.record().indexOf("StartTime");
        QString startTime = query.value(idName).toString();
        idName = query.record().indexOf("EndTime");
        QString endTime = query.value(idName).toString();
        idName = query.record().indexOf("Category");
        QString category = query.value(idName).toString();

        idName = query.record().indexOf("IsFullDay");
        int isFullDay = query.value(idName).toInt();

        idName = query.record().indexOf("IsRepeating");
        int isRepeating = query.value(idName).toInt();

        idName = query.record().indexOf("AddBirthday");
        int addBirthday = query.value(idName).toInt();

        idName = query.record().indexOf("ParentId");
        int parentId = query.value(idName).toInt();

        idName = query.record().indexOf("HasReminder");
        int hasReminder = query.value(idName).toInt();

        QDate appointmentDate =QDate::fromString(date);
        if(appointmentDate ==checkDate)
        {
            Appointment tmp;
            tmp.m_id=id;
            tmp.m_title=title;
            tmp.m_location=location;
            tmp.m_description=description;
            tmp.m_date=date;
            tmp.m_startTime=startTime;
            tmp.m_endTime=endTime;
            tmp.m_category=category;
            tmp.m_isFullDay=isFullDay;
            tmp.m_isRepeating=isRepeating;
            tmp.m_addBirthday=addBirthday;
            tmp.m_parentId=parentId;
            tmp.m_hasReminder=hasReminder;

            appointmentList.append(tmp);
        }
    }
    return appointmentList;
}


//-----------------------------------------------------------------------
//Contacts
//-----------------------------------------------------------------------


bool DbManager::addContact(Contact &contact)
{
    QString firstName =contact.m_firstname;
    QString midName=contact.m_midnames;
    QString lastName =contact.m_lastname;
    QString email =contact.m_email;
    QString street =contact.m_street;
    QString district=contact.m_district;
    QString city =contact.m_city;
    QString county =contact.m_county;
    QString postcode = contact.m_postcode;
    QString country =contact.m_country;
    QString telephone = contact.m_telephone;    
    QString birthdate=contact.m_birthdate;
    int addbirthday =contact.m_addbirthday;
    int appointmentid =contact.m_birthdayAppointmentId;

    QString sql="INSERT INTO contacts(`ContactId`,";
    sql.append("`FirstName`,");
    sql.append("`MidName`,");
    sql.append("`LastName`,");
    sql.append("`Email`,");
    sql.append("`Street`,");
     sql.append("`District`,");
    sql.append("`City`,");
    sql.append("`County`,");
    sql.append("`Postcode`,");
    sql.append("`Country`,");
    sql.append("`Telephone`,");
    sql.append("`BirthDate`,");
    sql.append("`AddBirthday`,");
    sql.append("`AppointmentId`)");
    sql.append("VALUES (:idin,");
    sql.append(":firstnamein,");
    sql.append(":midnamein,");
    sql.append(":lastnamein,");
    sql.append(":emailin,");
    sql.append(":streetin,");
    sql.append(":districtin,");
    sql.append(":cityin,");
    sql.append(":countyin,");
    sql.append(":postcodein,");
    sql.append(":countryin,");
    sql.append(":telephonein,");    
    sql.append(":birthdatein,");
    sql.append(":addbirthdayin,");
    sql.append(":appointmentidin);");

    QSqlQuery query;
    query.prepare(sql);

    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":firstnamein", firstName);
    query.bindValue(":midnamein", midName);
    query.bindValue(":lastnamein", lastName);
    query.bindValue(":emailin", email);
    query.bindValue(":streetin", street);
    query.bindValue(":districtin", district);
    query.bindValue(":cityin", city);
    query.bindValue(":countyin", county);
    query.bindValue(":postcodein", postcode);
    query.bindValue(":countryin", country);
    query.bindValue(":telephonein", telephone);    
    query.bindValue(":birthdatein", birthdate);
    query.bindValue(":addbirthdayin", addbirthday);
    query.bindValue(":appointmentidin", appointmentid);
    bool success = query.exec();
    if (success) {
        //qDebug() << "added new contact";
        return success;
    } else {
        //qDebug() << "new contact add failed" << query.lastError();
        return success;
    }
}

bool DbManager::updateContact(Contact &contact, int id)
{
    bool success =false;


    QString sql ="UPDATE contacts SET ";
    sql.append(" FirstName = :fnin");
    sql.append(", MidName = :mnin");
    sql.append(", LastName = :lnin");
    sql.append(", Email = :emin");
    sql.append(", Street = :stin");
    sql.append(", District = :disin");
    sql.append(", City = :ctyin");
    sql.append(", County = :cntin");
    sql.append(", Postcode = :pcin");
    sql.append(", Country =:cin");
    sql.append(", Telephone =:tin");    
    sql.append(", BirthDate =:bdatein");
    sql.append(", AddBirthday =:addbirthdayin");
    //sql.append(", AppointmentId =:bidin");
    sql.append(" WHERE ContactId =:idin");

    //qDebug()<<"sql query = "<<sql;

    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(":fnin", contact.m_firstname);
      qry.bindValue(":mnin",contact.m_midnames);
      qry.bindValue(":lnin",contact.m_lastname);
      qry.bindValue(":emin",contact.m_email);
      qry.bindValue(":stin",contact.m_street);
      qry.bindValue(":disin",contact.m_district);
      qry.bindValue(":ctyin",contact.m_city);
      qry.bindValue(":cntin",contact.m_county);
      qry.bindValue(":pcin",contact.m_postcode);
      qry.bindValue(":cin",contact.m_country);
      qry.bindValue(":tin",contact.m_telephone);      
      qry.bindValue(":bdatein",contact.m_birthdate);
      qry.bindValue(":addbirthdayin",contact.m_addbirthday);
      //qry.bindValue(":bidin",contact.m_birthdayAppointmentId);
      qry.bindValue(":idin", QString::number(id));
      success = qry.exec();
      if (success)
      {
          //qDebug() << "Contact updated";
          return success;
      }
      else
      {
          //qDebug() << "Contact update failed";
          return success;
      }
    }
    else
    {
      //qDebug() << "SQL is broken";
      return success;
    }
}



QList<Contact> DbManager::getAllContacts()
{
    QList<Contact> contactList;   
    QSqlQuery query("SELECT * FROM contacts");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("ContactId");
        int id = query.value(idName).toInt();
        idName = query.record().indexOf("FirstName");
        QString firstname = query.value(idName).toString();        
        idName = query.record().indexOf("MidName");
        QString midname = query.value(idName).toString();
        idName = query.record().indexOf("LastName");
        QString lastname = query.value(idName).toString();        
        idName = query.record().indexOf("Email");
        QString email = query.value(idName).toString();
        idName = query.record().indexOf("Street");
        QString street = query.value(idName).toString();        
        idName = query.record().indexOf("District");
        QString district = query.value(idName).toString();
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
        idName = query.record().indexOf("BirthDate");
        QString birthdate = query.value(idName).toString();

        idName = query.record().indexOf("AddBirthday");
        int addbirthday= query.value(idName).toInt();

        idName = query.record().indexOf("AppointmentId");
        int appointmentid= query.value(idName).toInt();

        Contact tmp;
        tmp.m_id=id;
        tmp.m_firstname=firstname;
        tmp.m_midnames=midname;
        tmp.m_lastname=lastname;
        tmp.m_email=email;
        tmp.m_street=street;
        tmp.m_district=district;
        tmp.m_city=city;
        tmp.m_county=county;
        tmp.m_postcode=postcode;
        tmp.m_country=country;
        tmp.m_telephone=telephone;       
        tmp.m_birthdate=birthdate;
        tmp.m_addbirthday=addbirthday;
        tmp.m_birthdayAppointmentId=appointmentid;
        contactList.append(tmp);
    }
    return contactList;
}

Contact DbManager::getContactByID(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM contacts WHERE ContactId = (:idin)");
    query.bindValue(":idin", id);
    query.exec();
    query.first();

    int idName = query.record().indexOf("ContactId");
    int idd = query.value(idName).toInt();
    idName = query.record().indexOf("FirstName");
    QString firstname = query.value(idName).toString();
    idName = query.record().indexOf("MidName");
    QString midname = query.value(idName).toString();
    idName = query.record().indexOf("LastName");
    QString lastname = query.value(idName).toString();
    idName = query.record().indexOf("Email");
    QString email = query.value(idName).toString();
    idName = query.record().indexOf("Street");
    QString street = query.value(idName).toString();
    idName = query.record().indexOf("District");
    QString district = query.value(idName).toString();
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
    idName = query.record().indexOf("BirthDate");
    QString birthdate = query.value(idName).toString();

    idName = query.record().indexOf("AddBirthday");
    int addbirthday= query.value(idName).toInt();
    idName = query.record().indexOf("AppointmentId");
    int appointmentid= query.value(idName).toInt();

    Contact contact;
    contact.m_id=idd;
    contact.m_firstname=firstname;
    contact.m_midnames=midname;
    contact.m_lastname=lastname;
    contact.m_email=email;
    contact.m_street=street;
    contact.m_district=district;
    contact.m_city=city;
    contact.m_county=county;
    contact.m_postcode=postcode;
    contact.m_country=country;
    contact.m_telephone=telephone;    
    contact.m_birthdate=birthdate;
    contact.m_addbirthday=addbirthday;
    contact.m_birthdayAppointmentId=appointmentid;
    return contact;
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



