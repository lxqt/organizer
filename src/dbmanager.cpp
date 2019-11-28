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
    sql1.append("ReminderId INTEGER");
    sql1.append(");");

    QString sql2 = "CREATE TABLE reminders (ReminderId INTEGER PRIMARY KEY,";
    sql2.append("Details TEXT,");
    sql2.append("ReminderDate TEXT,");
    sql2.append("ReminderTime TEXT,");
    sql2.append("ReminderRequest INTEGER");
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
    sql3.append("BirthdayId INTEGER");
    sql3.append(");");


    QString sql4 = "CREATE TABLE birthdays (BirthdayId INTEGER PRIMARY KEY,";
    sql4.append("Name TEXT,");
    sql4.append("Location TEXT,");
    sql4.append("Description TEXT,");
    sql4.append("BirthDate TEXT,");
    sql4.append("AddToCalendar INTEGER");
    sql4.append(");");


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

    query.prepare(sql4);
    if(!query.exec())
    {
       // qDebug()<<"Table birthdays already exists";
    }
    else {
       // qDebug()<<"Table birthdays successfully created";
    }
}

bool DbManager::removeAllAppointments()
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
       // qDebug() << "remove appointment failed: " << queryDelete.lastError();
    }
    //qDebug()<< "Remove appointment success = "<<success;
    return success;
}

bool DbManager::addAppointment(Appointment &appointment)
{
    QString title=appointment.m_title;
    QString location=appointment.m_location;
    QString description =appointment.m_description;
    QString date = appointment.m_appointmentDate;
    QString startTime=appointment.m_appointmentStartTime;
    QString endTime=appointment.m_appointmentEndTime;
    int reminderId=appointment.m_reminderId;

    QString sql="INSERT INTO appointments(`AppointmentId`,";
    sql.append("`Title`,");
    sql.append("`Location`,");
    sql.append("`Description`,");
    sql.append("`Date`,");
    sql.append("`StartTime`,");
    sql.append("`EndTime`,");
    sql.append("`ReminderId`)");
    sql.append("VALUES (:idin,");
    sql.append(":titlein,");
    sql.append(":locationin,");
    sql.append(":descriptionin,");
    sql.append(":datein,");
    sql.append(":starttimein,");
    sql.append(":endtimein,");
    sql.append(":reminderidin);");

    QSqlQuery query;
    query.prepare(sql);
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":titlein", title);
    query.bindValue(":locationin",location);
    query.bindValue(":descriptionin",description);
    query.bindValue(":datein",date);
    query.bindValue(":starttimein",startTime);
    query.bindValue(":endtimein",endTime);
    query.bindValue(":reminderidin",reminderId);

    bool success = query.exec();
    if (success) {
        //qDebug() << "added new appointment";
        return success;
    } else {
        //qDebug() << "new appointent add failed" << query.lastError();
        return success;
    }
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
   // sql.append(", ReminderId = :ridin");
    sql.append(" WHERE AppointmentId =:idin");

    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(":tin", appointment.m_title);
      qry.bindValue(":lin", appointment.m_location);
      qry.bindValue(":din", appointment.m_description);
      qry.bindValue(":datein", appointment.m_appointmentDate);
      qry.bindValue(":stin",appointment.m_appointmentStartTime);
      qry.bindValue(":etin",appointment.m_appointmentEndTime);
    //  qry.bindValue(":ridin",appointment.m_reminderId);
      qry.bindValue(":idin", QString::number(id));
      success=qry.exec();
      if (success)
      {
        //qDebug() << "Appointment updated" << " sucess ="<<success;
        return success;  //true
      }
      else
      {
        //qDebug() << "Appointment update failed"<<" sucess ="<<success;
        return success;
      }
    }
    else
    {
      //qDebug() << "SQL is broken"<<" sucess ="<<success;
      return success;
    }
}

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

    int birthdayid =contact.m_birthdayid;

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
    sql.append("`BirthdayId`)");
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
    sql.append(":birthdayidin);");

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
    query.bindValue(":birthdayidin", birthdayid);
    bool success = query.exec();
    if (success) {
        //qDebug() << "added new contact";
        return success;
    } else {
        //qDebug() << "new contact add failed" << query.lastError();
        return success;
    }
}

bool DbManager::updateContact(Contact &contact,int id)
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
    //sql.append(", BirthdayId =:bidin");
    sql.append(" WHERE ContactId =:idin");

    qDebug()<<"sql query = "<<sql;

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
      //qry.bindValue(":bidin",contact.m_birthdayid);
      qry.bindValue(":idin", QString::number(id));
      success = qry.exec();
      if (success)
      {
          qDebug() << "Contact updated";
          return success;
      }
      else
      {
          qDebug() << "Contact update failed";
          return success;
      }
    }
    else
    {
      qDebug() << "SQL is broken";
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
        idName = query.record().indexOf("Date");
        QString date = query.value(idName).toString();
        idName = query.record().indexOf("StartTime");
        QString startTime = query.value(idName).toString();
        idName = query.record().indexOf("EndTime");
        QString endTime = query.value(idName).toString();
        idName = query.record().indexOf("ReminderId");
        int reminderId = query.value(idName).toInt();

        Appointment tmp;
        tmp.m_id=id;
        tmp.m_title=title;
        tmp.m_location=location;
        tmp.m_description=description;
        tmp.m_appointmentDate=date;
        tmp.m_appointmentStartTime=startTime;
        tmp.m_appointmentEndTime=endTime;
        tmp.m_reminderId=reminderId;
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
    idName = query.record().indexOf("ReminderId");
    int reminderId = query.value(idName).toInt();

    Appointment appointment;
    appointment.m_id=idd;
    appointment.m_title=title;
    appointment.m_location=location;
    appointment.m_description=description;
    appointment.m_appointmentDate=date;
    appointment.m_appointmentStartTime=startTime;
    appointment.m_appointmentEndTime=endTime;
    appointment.m_reminderId=reminderId;

    return appointment;

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
        idName = query.record().indexOf("BirthdayId");
        int birthdayid= query.value(idName).toInt();

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
        tmp.m_birthdayid=birthdayid;
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
    idName = query.record().indexOf("BirthdayId");
    int birthdayid= query.value(idName).toInt();

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
    contact.m_birthdayid=birthdayid;
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


int DbManager::addBirthday(Birthday &b)
{
    QString name =b.m_name;
    QString location =b.m_location;
    QString description=b.m_description;
    QString birthdate=b.m_birthDate;
    int addtocalendar=b.m_addToCalendar;

    QString sql="INSERT INTO birthdays(`BirthdayId`,";
    sql.append("`Name`,");
    sql.append("`Location`,");
    sql.append("`Description`,");
    sql.append("`BirthDate`,");
    sql.append("`AddToCalendar`)");
    sql.append("VALUES (:idin,");
    sql.append(":namein,");
    sql.append(":locationin,");
    sql.append(":descriptionin,");
    sql.append(":datein,");
    sql.append(":atcin);");

    QSqlQuery query;
    query.prepare(sql);
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":namein", name);
    query.bindValue(":locationin",location);
    query.bindValue(":descriptionin",description);
    query.bindValue(":datein",birthdate);
    query.bindValue(":atcin",addtocalendar);
    query.exec();
    int id = query.lastInsertId().toInt();   
    return id;
}

bool DbManager::updateBirthday(Birthday &birthday, int id)
{
    bool success =false;

    QString sql ="UPDATE birthdays SET ";
    sql.append(" Name = :nin");
    sql.append(", Location = :lin");
    sql.append(", Description = :din");
    sql.append(", BirthDate = :datein");
    sql.append(", AddToCalendar = :atcin");
    sql.append(" WHERE BirthdayId =:idin");

    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(":nin", birthday.m_name);
      qry.bindValue(":lin", birthday.m_location);
      qry.bindValue(":din", birthday.m_description);
      qry.bindValue(":datein", birthday.m_birthDate);
      qry.bindValue(":atcin", birthday.m_addToCalendar);
      qry.bindValue(":idin", QString::number(id));
      success=qry.exec();
      if (success)
      {
        //qDebug() << "Birthday updated" << " success ="<<success;
        return success;  //true
      }
      else
      {
        //qDebug() << "Birthday update failed"<<" sucess ="<<success;
        return success;
      }
    }
    else
    {
      qDebug() << "SQL is broken"<<" sucess ="<<success;
      return success;
    }
}

QList<Birthday> DbManager::getAllBirthdays()
{
    QList<Birthday> birthdayList;    
    QSqlQuery query("SELECT * FROM birthdays");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("BirthdayId");
        int id = query.value(idName).toInt();
        idName = query.record().indexOf("Name");
        QString name = query.value(idName).toString();
        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();
        idName = query.record().indexOf("Description");
        QString description = query.value(idName).toString();
        idName = query.record().indexOf("BirthDate");
        QString birthdate = query.value(idName).toString();
        idName = query.record().indexOf("AddToCalendar");
        int addtocalendar = query.value(idName).toInt();
        //Create a temporatory appointment
        Birthday tmp;
        tmp.m_id=id;
        tmp.m_name=name;
        tmp.m_location=location;
        tmp.m_description=description;
        tmp.m_birthDate=birthdate;
        tmp.m_addToCalendar=addtocalendar;
        birthdayList.append(tmp);
}
    return birthdayList;
}

Birthday DbManager::getBirthdayByID(int id)
{   
    QSqlQuery query;
    query.prepare("SELECT * FROM birthdays WHERE BirthdayId = (:idin)");
    query.bindValue(":idin", id);
    query.exec();
    query.first();

    int idName = query.record().indexOf("BirthdayId");
    int idd = query.value(idName).toInt();
    idName = query.record().indexOf("Name");
    QString name = query.value(idName).toString();
    idName = query.record().indexOf("Location");
    QString location = query.value(idName).toString();
    idName = query.record().indexOf("Description");
    QString description = query.value(idName).toString();
    idName = query.record().indexOf("BirthDate");
    QString birthdate = query.value(idName).toString();
    idName = query.record().indexOf("AddToCalendar");
    int addtocalendar = query.value(idName).toInt();
    //Create a temporatory birthday
    Birthday b;
    b.m_id=idd;
    b.m_name=name;
    b.m_location=location;
    b.m_description=description;
    b.m_birthDate=birthdate;
    b.m_addToCalendar=addtocalendar;
    return b;
}

QList<Birthday> DbManager::getBirthdaysOnDate(QDate &diaryDate)
{
    QDate checkDate = QDate(diaryDate);

    QList<Birthday> birthdayList = QList<Birthday>(); //Construct an empty list
    QSqlQuery query("SELECT * FROM birthdays");
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf("BirthdayId");
        int id = query.value(idName).toInt();
        idName = query.record().indexOf("Name");
        QString name = query.value(idName).toString();
        idName = query.record().indexOf("Location");
        QString location = query.value(idName).toString();
        idName = query.record().indexOf("Description");
        QString description = query.value(idName).toString();
        idName = query.record().indexOf("BirthDate");
        QString birthdate = query.value(idName).toString();
        idName = query.record().indexOf("AddToCalendar");
        int addtocalendar = query.value(idName).toInt();
        QDate currentDate=QDate::currentDate();
        QDate bornDate=QDate::fromString(birthdate);
        QDate theBirthdayDate =QDate(currentDate.year(),bornDate.month(),bornDate.day());
        if(theBirthdayDate ==checkDate)
        {
            //Create a temporatory birthday
            Birthday tmp;
            tmp.m_id=id;
            tmp.m_name=name;
            tmp.m_location=location;
            tmp.m_description=description;
            tmp.m_birthDate=birthdate;
            tmp.m_addToCalendar=addtocalendar;
            birthdayList.append(tmp);
        }
    }
    return birthdayList;
}

bool DbManager::removeBirthdayById(const int id)
{
    bool success = false;   
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM birthdays WHERE BirthdayId = (:idin)");
    queryDelete.bindValue(":idin", id);
    success = queryDelete.exec();
    if(!success)
    {
        //qDebug() << "remove birthday failed: " << queryDelete.lastError();
    }
    //qDebug()<< "Remove birthday success = "<<success;
    return success;
}

bool DbManager::removeAllBirthdays()
{
    bool success = false;
    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM birthdays");
    if (removeQuery.exec())
    {
        //qDebug() << "remove all birthdays succeeded";
        success = true;
    }
    else
    {
        //qDebug() << "remove all birthdays failed: " << removeQuery.lastError();
    }
    return success;
}

int DbManager::addReminder(Reminder &reminder)
{
    QString details =reminder.m_details;
    QString reminderdate =reminder.m_reminderDate;
    QString remindertime =reminder.m_reminderTime;
    int reminderrequest =reminder.m_reminderRequest;

    QString sql="INSERT INTO reminders(`ReminderId`,";
    sql.append("`Details`,");
    sql.append("`ReminderDate`,");
    sql.append("`ReminderTime`,");
    sql.append("`ReminderRequest`)");
    sql.append("VALUES (:idin,");
    sql.append(":detailsin,");
    sql.append(":reminderdatein,");
    sql.append(":remindertimein,");
    sql.append(":reminderrequestin);");

    QSqlQuery query;
    query.prepare(sql);
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(":detailsin", details);
    query.bindValue(":reminderdatein",reminderdate);
    query.bindValue(":remindertimein",remindertime);
    query.bindValue(":reminderrequestin",reminderrequest);
    query.exec();
    int id = query.lastInsertId().toInt();    
    return id;
}

bool DbManager::updateReminder(Reminder &reminder, int id)
{
    bool success =false;
    QString sql ="UPDATE reminders SET ";
    sql.append(" Details = :din");
    sql.append(", ReminderDate = :rdin");
    sql.append(", ReminderTime = :rtin");
    sql.append(", ReminderRequest = :rrin");
    sql.append(" WHERE ReminderId =:idin");
    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(":din", reminder.m_details);
      qry.bindValue(":rdin", reminder.m_reminderDate);
      qry.bindValue(":rtin", reminder.m_reminderTime);
      qry.bindValue(":rrin", reminder.m_reminderRequest);
      qry.bindValue(":idin", QString::number(id));
      success=qry.exec();
      if (success)
      {
        qDebug() << "Reminder updated" << " success ="<<success;
        return success;  //true
      }
      else
      {
        qDebug() << "Reminder update failed"<<" sucess ="<<success;
        return success;
      }
    }
    else
    {
      qDebug() << "SQL is broken"<<" sucess ="<<success;
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
        int idName = query.record().indexOf("ReminderId");
        int id = query.value(idName).toInt();
        idName = query.record().indexOf("Details");
        QString details = query.value(idName).toString();
        idName = query.record().indexOf("ReminderDate");
        QString reminderDate = query.value(idName).toString();
        idName = query.record().indexOf("ReminderTime");
        QString reminderTime = query.value(idName).toString();
        idName = query.record().indexOf("ReminderRequest");
        int reminderRequest = query.value(idName).toInt();
        //Create a temporary reminder
        Reminder r;
        r.m_id=id;
        r.m_details=details;
        r.m_reminderDate=reminderDate;
        r.m_reminderTime= reminderTime;
        r.m_reminderRequest=reminderRequest;
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
    int idName = query.record().indexOf("ReminderId");
    int idd = query.value(idName).toInt();
    idName = query.record().indexOf("Details");
    QString details = query.value(idName).toString();
    idName = query.record().indexOf("ReminderDate");
    QString reminderdate = query.value(idName).toString();
    idName = query.record().indexOf("ReminderTime");
    QString remindertime = query.value(idName).toString();
    idName = query.record().indexOf("ReminderRequest");
    int reminderrequest = query.value(idName).toInt();
    //Create a reminder and return it
    Reminder r;
    r.m_id=idd;
    r.m_details=details;
    r.m_reminderDate=reminderdate;
    r.m_reminderTime=remindertime;
    r.m_reminderRequest=reminderrequest;
    return r;
}

bool DbManager::removeReminderById(const int id)
{
    bool success = false;    
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM reminders WHERE ReminderId = (:idin)");
    queryDelete.bindValue(":idin", id);
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
        idName = query.record().indexOf("ReminderId");
        int reminderId = query.value(idName).toInt();

        QDate appointmentDate =QDate::fromString(date);
        if(appointmentDate ==checkDate)
        {
            Appointment tmp;
            tmp.m_id=id;
            tmp.m_title=title;
            tmp.m_location=location;
            tmp.m_description=description;
            tmp.m_appointmentDate=date;
            tmp.m_appointmentStartTime=startTime;
            tmp.m_appointmentEndTime=endTime;
            tmp.m_reminderId=reminderId;
            appointmentList.append(tmp);
        }
    }
    return appointmentList;
}
