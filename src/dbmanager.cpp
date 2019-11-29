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
    if(QSqlDatabase::isDriverAvailable(QStringLiteral("QSQLITE")))
    {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));

        db.setDatabaseName(QStringLiteral("organizerdb.sqlite"));
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

    QString sql1 = QStringLiteral("CREATE TABLE appointments (AppointmentId INTEGER PRIMARY KEY,");
    sql1.append(QLatin1String("Title TEXT,"));
    sql1.append(QLatin1String("Location TEXT,"));
    sql1.append(QLatin1String("Description TEXT,"));
    sql1.append(QLatin1String("Date TEXT,"));
    sql1.append(QLatin1String("StartTime TEXT,"));
    sql1.append(QLatin1String("EndTime TEXT,"));
    sql1.append(QLatin1String("ReminderId INTEGER"));
    sql1.append(QLatin1String(");"));

    QString sql2 = QStringLiteral("CREATE TABLE reminders (ReminderId INTEGER PRIMARY KEY,");
    sql2.append(QLatin1String("Details TEXT,"));
    sql2.append(QLatin1String("ReminderDate TEXT,"));
    sql2.append(QLatin1String("ReminderTime TEXT,"));
    sql2.append(QLatin1String("ReminderRequest INTEGER"));
    sql2.append(QLatin1String(");"));

    QString sql3 = QStringLiteral("CREATE TABLE contacts (ContactId INTEGER PRIMARY KEY,");
    sql3.append(QLatin1String("FirstName TEXT,"));
    sql3.append(QLatin1String("MidName TEXT,"));
    sql3.append(QLatin1String("LastName TEXT,"));
    sql3.append(QLatin1String("Email TEXT,"));
    sql3.append(QLatin1String("Street TEXT,"));
    sql3.append(QLatin1String("District TEXT,"));
    sql3.append(QLatin1String("City TEXT,"));
    sql3.append(QLatin1String("County TEXT,"));
    sql3.append(QLatin1String("Postcode TEXT,"));
    sql3.append(QLatin1String("Country TEXT,"));
    sql3.append(QLatin1String("Telephone TEXT,"));
    sql3.append(QLatin1String("BirthDate TEXT,"));
    sql3.append(QLatin1String("BirthdayId INTEGER"));
    sql3.append(QLatin1String(");"));


    QString sql4 = QStringLiteral("CREATE TABLE birthdays (BirthdayId INTEGER PRIMARY KEY,");
    sql4.append(QLatin1String("Name TEXT,"));
    sql4.append(QLatin1String("Location TEXT,"));
    sql4.append(QLatin1String("Description TEXT,"));
    sql4.append(QLatin1String("BirthDate TEXT,"));
    sql4.append(QLatin1String("AddToCalendar INTEGER"));
    sql4.append(QLatin1String(");"));


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
    removeQuery.prepare(QStringLiteral("DELETE FROM appointments"));
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
    queryDelete.prepare(QStringLiteral("DELETE FROM appointments WHERE AppointmentId = (:idin)"));
    queryDelete.bindValue(QStringLiteral(":idin"), id);
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

    QString sql=QStringLiteral("INSERT INTO appointments(`AppointmentId`,");
    sql.append(QLatin1String("`Title`,"));
    sql.append(QLatin1String("`Location`,"));
    sql.append(QLatin1String("`Description`,"));
    sql.append(QLatin1String("`Date`,"));
    sql.append(QLatin1String("`StartTime`,"));
    sql.append(QLatin1String("`EndTime`,"));
    sql.append(QLatin1String("`ReminderId`)"));
    sql.append(QLatin1String("VALUES (:idin,"));
    sql.append(QLatin1String(":titlein,"));
    sql.append(QLatin1String(":locationin,"));
    sql.append(QLatin1String(":descriptionin,"));
    sql.append(QLatin1String(":datein,"));
    sql.append(QLatin1String(":starttimein,"));
    sql.append(QLatin1String(":endtimein,"));
    sql.append(QLatin1String(":reminderidin);"));

    QSqlQuery query;
    query.prepare(sql);
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(QStringLiteral(":titlein"), title);
    query.bindValue(QStringLiteral(":locationin"),location);
    query.bindValue(QStringLiteral(":descriptionin"),description);
    query.bindValue(QStringLiteral(":datein"),date);
    query.bindValue(QStringLiteral(":starttimein"),startTime);
    query.bindValue(QStringLiteral(":endtimein"),endTime);
    query.bindValue(QStringLiteral(":reminderidin"),reminderId);

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

    QString sql =QStringLiteral("UPDATE appointments SET ");
    sql.append(QLatin1String(" Title = :tin"));
    sql.append(QLatin1String(", Location = :lin"));
    sql.append(QLatin1String(", Description = :din"));
    sql.append(QLatin1String(", Date = :datein"));
    sql.append(QLatin1String(", StartTime = :stin"));
    sql.append(QLatin1String(", EndTime = :etin"));
   // sql.append(QLatin1String(", ReminderId = :ridin"));
    sql.append(QLatin1String(" WHERE AppointmentId =:idin"));

    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(QStringLiteral(":tin"), appointment.m_title);
      qry.bindValue(QStringLiteral(":lin"), appointment.m_location);
      qry.bindValue(QStringLiteral(":din"), appointment.m_description);
      qry.bindValue(QStringLiteral(":datein"), appointment.m_appointmentDate);
      qry.bindValue(QStringLiteral(":stin"),appointment.m_appointmentStartTime);
      qry.bindValue(QStringLiteral(":etin"),appointment.m_appointmentEndTime);
    //  qry.bindValue(QStringLiteral(":ridin"),appointment.m_reminderId);
      qry.bindValue(QStringLiteral(":idin"), QString::number(id));
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

    QString sql=QStringLiteral("INSERT INTO contacts(`ContactId`,");
    sql.append(QLatin1String("`FirstName`,"));
    sql.append(QLatin1String("`MidName`,"));
    sql.append(QLatin1String("`LastName`,"));
    sql.append(QLatin1String("`Email`,"));
    sql.append(QLatin1String("`Street`,"));
     sql.append(QLatin1String("`District`,"));
    sql.append(QLatin1String("`City`,"));
    sql.append(QLatin1String("`County`,"));
    sql.append(QLatin1String("`Postcode`,"));
    sql.append(QLatin1String("`Country`,"));
    sql.append(QLatin1String("`Telephone`,"));
    sql.append(QLatin1String("`BirthDate`,"));
    sql.append(QLatin1String("`BirthdayId`)"));
    sql.append(QLatin1String("VALUES (:idin,"));
    sql.append(QLatin1String(":firstnamein,"));
    sql.append(QLatin1String(":midnamein,"));
    sql.append(QLatin1String(":lastnamein,"));
    sql.append(QLatin1String(":emailin,"));
    sql.append(QLatin1String(":streetin,"));
    sql.append(QLatin1String(":districtin,"));
    sql.append(QLatin1String(":cityin,"));
    sql.append(QLatin1String(":countyin,"));
    sql.append(QLatin1String(":postcodein,"));
    sql.append(QLatin1String(":countryin,"));
    sql.append(QLatin1String(":telephonein,"));
    sql.append(QLatin1String(":birthdatein,"));
    sql.append(QLatin1String(":birthdayidin);"));

    QSqlQuery query;
    query.prepare(sql);

    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(QStringLiteral(":firstnamein"), firstName);
    query.bindValue(QStringLiteral(":midnamein"), midName);
    query.bindValue(QStringLiteral(":lastnamein"), lastName);
    query.bindValue(QStringLiteral(":emailin"), email);
    query.bindValue(QStringLiteral(":streetin"), street);
    query.bindValue(QStringLiteral(":districtin"), district);
    query.bindValue(QStringLiteral(":cityin"), city);
    query.bindValue(QStringLiteral(":countyin"), county);
    query.bindValue(QStringLiteral(":postcodein"), postcode);
    query.bindValue(QStringLiteral(":countryin"), country);
    query.bindValue(QStringLiteral(":telephonein"), telephone);    
    query.bindValue(QStringLiteral(":birthdatein"), birthdate);
    query.bindValue(QStringLiteral(":birthdayidin"), birthdayid);
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

    QString sql =QStringLiteral("UPDATE contacts SET ");
    sql.append(QLatin1String(" FirstName = :fnin"));
    sql.append(QLatin1String(", MidName = :mnin"));
    sql.append(QLatin1String(", LastName = :lnin"));
    sql.append(QLatin1String(", Email = :emin"));
    sql.append(QLatin1String(", Street = :stin"));
    sql.append(QLatin1String(", District = :disin"));
    sql.append(QLatin1String(", City = :ctyin"));
    sql.append(QLatin1String(", County = :cntin"));
    sql.append(QLatin1String(", Postcode = :pcin"));
    sql.append(QLatin1String(", Country =:cin"));
    sql.append(QLatin1String(", Telephone =:tin"));
    sql.append(QLatin1String(", BirthDate =:bdatein"));
    //sql.append(QLatin1String(", BirthdayId =:bidin"));
    sql.append(QLatin1String(" WHERE ContactId =:idin"));

    qDebug()<<"sql query = "<<sql;

    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(QStringLiteral(":fnin"), contact.m_firstname);
      qry.bindValue(QStringLiteral(":mnin"),contact.m_midnames);
      qry.bindValue(QStringLiteral(":lnin"),contact.m_lastname);
      qry.bindValue(QStringLiteral(":emin"),contact.m_email);
      qry.bindValue(QStringLiteral(":stin"),contact.m_street);
      qry.bindValue(QStringLiteral(":disin"),contact.m_district);
      qry.bindValue(QStringLiteral(":ctyin"),contact.m_city);
      qry.bindValue(QStringLiteral(":cntin"),contact.m_county);
      qry.bindValue(QStringLiteral(":pcin"),contact.m_postcode);
      qry.bindValue(QStringLiteral(":cin"),contact.m_country);
      qry.bindValue(QStringLiteral(":tin"),contact.m_telephone);      
      qry.bindValue(QStringLiteral(":bdatein"),contact.m_birthdate);
      //qry.bindValue(":bidin",contact.m_birthdayid);
      qry.bindValue(QStringLiteral(":idin"), QString::number(id));
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
    QSqlQuery query(QStringLiteral("SELECT * FROM appointments"));
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf(QStringLiteral("AppointmentId"));
        int id = query.value(idName).toInt();
        idName = query.record().indexOf(QStringLiteral("Title"));
        QString title = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Location"));
        QString location = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Description"));
        QString description = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Date"));
        QString date = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("StartTime"));
        QString startTime = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("EndTime"));
        QString endTime = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("ReminderId"));
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
    query.prepare(QStringLiteral("SELECT * FROM appointments WHERE AppointmentId = (:idin)"));
    query.bindValue(QStringLiteral(":idin"), id);
    query.exec();
    query.first();

    int idName = query.record().indexOf(QStringLiteral("AppointmentId"));
    int idd = query.value(idName).toInt();

    idName = query.record().indexOf(QStringLiteral("Title"));
    QString title = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Location"));
    QString location = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Description"));
    QString description = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Date"));
    QString date = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("StartTime"));
    QString startTime = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("EndTime"));
    QString endTime = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("ReminderId"));
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
    QSqlQuery query(QStringLiteral("SELECT * FROM contacts"));
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf(QStringLiteral("ContactId"));
        int id = query.value(idName).toInt();
        idName = query.record().indexOf(QStringLiteral("FirstName"));
        QString firstname = query.value(idName).toString();        
        idName = query.record().indexOf(QStringLiteral("MidName"));
        QString midname = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("LastName"));
        QString lastname = query.value(idName).toString();        
        idName = query.record().indexOf(QStringLiteral("Email"));
        QString email = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Street"));
        QString street = query.value(idName).toString();        
        idName = query.record().indexOf(QStringLiteral("District"));
        QString district = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("City"));
        QString city = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("County"));
        QString county = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Postcode"));
        QString postcode = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Country"));
        QString country = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Telephone"));
        QString telephone = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("BirthDate"));
        QString birthdate = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("BirthdayId"));
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
    query.prepare(QStringLiteral("SELECT * FROM contacts WHERE ContactId = (:idin)"));
    query.bindValue(QStringLiteral(":idin"), id);
    query.exec();
    query.first();

    int idName = query.record().indexOf(QStringLiteral("ContactId"));
    int idd = query.value(idName).toInt();
    idName = query.record().indexOf(QStringLiteral("FirstName"));
    QString firstname = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("MidName"));
    QString midname = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("LastName"));
    QString lastname = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Email"));
    QString email = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Street"));
    QString street = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("District"));
    QString district = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("City"));
    QString city = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("County"));
    QString county = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Postcode"));
    QString postcode = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Country"));
    QString country = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Telephone"));
    QString telephone = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("BirthDate"));
    QString birthdate = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("BirthdayId"));
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
    removeQuery.prepare(QStringLiteral("DELETE FROM contacts"));
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
    queryDelete.prepare(QStringLiteral("DELETE FROM contacts WHERE ContactId = (:idin)"));
    queryDelete.bindValue(QStringLiteral(":idin"), id);
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

    QString sql=QStringLiteral("INSERT INTO birthdays(`BirthdayId`,");
    sql.append(QLatin1String("`Name`,"));
    sql.append(QLatin1String("`Location`,"));
    sql.append(QLatin1String("`Description`,"));
    sql.append(QLatin1String("`BirthDate`,"));
    sql.append(QLatin1String("`AddToCalendar`)"));
    sql.append(QLatin1String("VALUES (:idin,"));
    sql.append(QLatin1String(":namein,"));
    sql.append(QLatin1String(":locationin,"));
    sql.append(QLatin1String(":descriptionin,"));
    sql.append(QLatin1String(":datein,"));
    sql.append(QLatin1String(":atcin);"));

    QSqlQuery query;
    query.prepare(sql);
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(QStringLiteral(":namein"), name);
    query.bindValue(QStringLiteral(":locationin"),location);
    query.bindValue(QStringLiteral(":descriptionin"),description);
    query.bindValue(QStringLiteral(":datein"),birthdate);
    query.bindValue(QStringLiteral(":atcin"),addtocalendar);
    query.exec();
    int id = query.lastInsertId().toInt();   
    return id;
}

bool DbManager::updateBirthday(Birthday &birthday, int id)
{
    bool success =false;

    QString sql =QStringLiteral("UPDATE birthdays SET ");
    sql.append(QLatin1String(" Name = :nin"));
    sql.append(QLatin1String(", Location = :lin"));
    sql.append(QLatin1String(", Description = :din"));
    sql.append(QLatin1String(", BirthDate = :datein"));
    sql.append(QLatin1String(", AddToCalendar = :atcin"));
    sql.append(QLatin1String(" WHERE BirthdayId =:idin"));

    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(QStringLiteral(":nin"), birthday.m_name);
      qry.bindValue(QStringLiteral(":lin"), birthday.m_location);
      qry.bindValue(QStringLiteral(":din"), birthday.m_description);
      qry.bindValue(QStringLiteral(":datein"), birthday.m_birthDate);
      qry.bindValue(QStringLiteral(":atcin"), birthday.m_addToCalendar);
      qry.bindValue(QStringLiteral(":idin"), QString::number(id));
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
    QSqlQuery query(QStringLiteral("SELECT * FROM birthdays"));
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf(QStringLiteral("BirthdayId"));
        int id = query.value(idName).toInt();
        idName = query.record().indexOf(QStringLiteral("Name"));
        QString name = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Location"));
        QString location = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Description"));
        QString description = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("BirthDate"));
        QString birthdate = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("AddToCalendar"));
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
    query.prepare(QStringLiteral("SELECT * FROM birthdays WHERE BirthdayId = (:idin)"));
    query.bindValue(QStringLiteral(":idin"), id);
    query.exec();
    query.first();

    int idName = query.record().indexOf(QStringLiteral("BirthdayId"));
    int idd = query.value(idName).toInt();
    idName = query.record().indexOf(QStringLiteral("Name"));
    QString name = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Location"));
    QString location = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("Description"));
    QString description = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("BirthDate"));
    QString birthdate = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("AddToCalendar"));
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
    QSqlQuery query(QStringLiteral("SELECT * FROM birthdays"));
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf(QStringLiteral("BirthdayId"));
        int id = query.value(idName).toInt();
        idName = query.record().indexOf(QStringLiteral("Name"));
        QString name = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Location"));
        QString location = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Description"));
        QString description = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("BirthDate"));
        QString birthdate = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("AddToCalendar"));
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
    queryDelete.prepare(QStringLiteral("DELETE FROM birthdays WHERE BirthdayId = (:idin)"));
    queryDelete.bindValue(QStringLiteral(":idin"), id);
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
    removeQuery.prepare(QStringLiteral("DELETE FROM birthdays"));
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

    QString sql=QStringLiteral("INSERT INTO reminders(`ReminderId`,");
    sql.append(QLatin1String("`Details`,"));
    sql.append(QLatin1String("`ReminderDate`,"));
    sql.append(QLatin1String("`ReminderTime`,"));
    sql.append(QLatin1String("`ReminderRequest`)"));
    sql.append(QLatin1String("VALUES (:idin,"));
    sql.append(QLatin1String(":detailsin,"));
    sql.append(QLatin1String(":reminderdatein,"));
    sql.append(QLatin1String(":remindertimein,"));
    sql.append(QLatin1String(":reminderrequestin);"));

    QSqlQuery query;
    query.prepare(sql);
    //query.bindValue(":idin", 1); //autoincrement as primary key
    query.bindValue(QStringLiteral(":detailsin"), details);
    query.bindValue(QStringLiteral(":reminderdatein"),reminderdate);
    query.bindValue(QStringLiteral(":remindertimein"),remindertime);
    query.bindValue(QStringLiteral(":reminderrequestin"),reminderrequest);
    query.exec();
    int id = query.lastInsertId().toInt();    
    return id;
}

bool DbManager::updateReminder(Reminder &reminder, int id)
{
    bool success =false;
    QString sql =QStringLiteral("UPDATE reminders SET ");
    sql.append(QLatin1String(" Details = :din"));
    sql.append(QLatin1String(", ReminderDate = :rdin"));
    sql.append(QLatin1String(", ReminderTime = :rtin"));
    sql.append(QLatin1String(", ReminderRequest = :rrin"));
    sql.append(QLatin1String(" WHERE ReminderId =:idin"));
    QSqlQuery qry;
    if(qry.prepare(sql))
    {
      qry.bindValue(QStringLiteral(":din"), reminder.m_details);
      qry.bindValue(QStringLiteral(":rdin"), reminder.m_reminderDate);
      qry.bindValue(QStringLiteral(":rtin"), reminder.m_reminderTime);
      qry.bindValue(QStringLiteral(":rrin"), reminder.m_reminderRequest);
      qry.bindValue(QStringLiteral(":idin"), QString::number(id));
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
    QSqlQuery query(QStringLiteral("SELECT * FROM reminders"));
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf(QStringLiteral("ReminderId"));
        int id = query.value(idName).toInt();
        idName = query.record().indexOf(QStringLiteral("Details"));
        QString details = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("ReminderDate"));
        QString reminderDate = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("ReminderTime"));
        QString reminderTime = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("ReminderRequest"));
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
    query.prepare(QStringLiteral("SELECT * FROM reminders WHERE ReminderId = (:idin)"));
    query.bindValue(QStringLiteral(":idin"), id);
    query.exec();
    query.first();
    int idName = query.record().indexOf(QStringLiteral("ReminderId"));
    int idd = query.value(idName).toInt();
    idName = query.record().indexOf(QStringLiteral("Details"));
    QString details = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("ReminderDate"));
    QString reminderdate = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("ReminderTime"));
    QString remindertime = query.value(idName).toString();
    idName = query.record().indexOf(QStringLiteral("ReminderRequest"));
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
    queryDelete.prepare(QStringLiteral("DELETE FROM reminders WHERE ReminderId = (:idin)"));
    queryDelete.bindValue(QStringLiteral(":idin"), id);
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
    removeQuery.prepare(QStringLiteral("DELETE FROM reminders"));
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
    QSqlQuery query(QStringLiteral("SELECT * FROM appointments"));
    query.exec();

    while (query.next())
    {
        int idName = query.record().indexOf(QStringLiteral("AppointmentId"));
        int id = query.value(idName).toInt();
        idName = query.record().indexOf(QStringLiteral("Title"));
        QString title = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Location"));
        QString location = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Description"));
        QString description = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("Date"));
        QString date = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("StartTime"));
        QString startTime = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("EndTime"));
        QString endTime = query.value(idName).toString();
        idName = query.record().indexOf(QStringLiteral("ReminderId"));
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
