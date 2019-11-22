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
#include "birthday.h"
#include "reminder.h"


class DbManager
{
public:
    DbManager();
    QSqlDatabase db;
    bool isOpen() const;
    void openDatabase();    
    void createDatebaseTables();
    bool addAppointment(Appointment &appointment);
    bool updateAppointment(Appointment &appointment, int id);

    QList<Appointment> getAllAppointments();
    Appointment getAppointmentByID(int id);
    QList<Appointment> getAppointmentsOnDate(QDate &diaryDate);    
    bool removeAppointmentById(const int id);
    bool removeAllAppointments();

    bool addContact(Contact &contact);
    bool updateContact(Contact &contact, int id);
    QList<Contact> getAllContacts();
    Contact getContactByID(int id);
    bool removeAllContacts();
    bool removeContactById(const int id);

    int addBirthday(Birthday &birthdate); //returns db index
    bool updateBirthday(Birthday &birthday, int id);
    QList<Birthday> getAllBirthdays();
    Birthday getBirthdayByID(int id);
    QList<Birthday> getBirthdaysOnDate(QDate &diaryDate);
    bool removeBirthdayById(const int id);
    bool removeAllBirthdays();


    int addReminder(Reminder &reminder);
    bool updateReminder(Reminder &reminder, int id);
    QList<Reminder> getAllReminders();
    Reminder getReminderByID(int id);
    QList<Reminder> getRemindersOnDate(QDate &diaryDate);
    bool removeReminderById(const int id);
    bool removeAllReminders();




};

#endif // DBMANAGER_H
