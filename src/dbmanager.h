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

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringListModel>
#include <QDebug>
#include <QDate>
#include <QList>
#include"appointment.h"
#include "contact.h"
#include "reminder.h"
#include "holiday.h"



class DbManager
{
public:
    DbManager();
    QSqlDatabase db;
    bool isOpen() const;
    void openDatabase();    
    void createDatebaseTables();

    //Appointments
    int addAppointment(Appointment &appointment);
    bool updateAppointment(Appointment &appointment, int id);
    QList<Appointment> getAllAppointments();
    Appointment getAppointmentByID(int id);
    QList<Appointment> getAppointmentsOnDate(QDate &diaryDate);    
    bool deleteAppointmentById(const int id);
    bool deleteAppointmentByParentID(const int parentId);
    bool deleteAllAppointments();

    //Reminders
    bool addReminder(Reminder &reminder);
    //bool updateReminder(Reminder &reminder, int id);
    bool updateReminder(Reminder &reminder, int id);
    QList<Reminder> getAllReminders();
    Reminder getReminderByID(int id);
    QList<Reminder> getRemindersOnDate(QDate &diaryDate);
    bool removeReminderById(const int appointmentId);
    bool removeAllReminders();

    //Contacts
    bool addContact(Contact &contact);
    bool updateContact(Contact &contact, int id);
    QList<Contact> getAllContacts();
    Contact getContactByID(int id);
    bool removeAllContacts();
    bool removeContactById(const int id);

    //Holidays
     bool addHoliday(Holiday &holiday); //returns db index
     QList<Holiday> getAllHolidays();
     Holiday getHolidayByID(int id);
     QList<Holiday> getHolidaysOnDate(QDate &diaryDate);
     bool removeHolidayById(const int id);
     bool removeAllHolidays();


};

#endif // DBMANAGER_H
