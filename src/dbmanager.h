/***************************************************************************
 *   Author Alan Crispin aka. crispina                 *
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
#include "appointment.h"
#include "contact.h"
#include "preferences.h"


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
    bool deleteAllAppointments();

    //Contacts
    int addContact(Contact &contact);
    bool updateContact(Contact &contact, int id);
    QList<Contact> getAllContacts();
    Contact getContactByID(int id);
    bool removeAllContacts();
    bool removeContactById(const int id);

    //Preferences (settings)
    int addPreferences(Preferences &preferences);
    bool updatePreferences(Preferences &preferences, int id);
    Preferences getPreferencesByID(int id);



};

#endif // DBMANAGER_H
