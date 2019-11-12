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


class DbManager
{
public:
    DbManager();
    QSqlDatabase db;
    bool isOpen() const;
    void openDatabase();    
    void createDatebaseTables();
    bool addAppointment(Appointment &appointment);
    bool addContact(Contact &contact);
    QList<Appointment> getAllAppointments();
    QList<Appointment> getAppointmentsOnDate(QDate &diaryDate);
    QList<Appointment> getAppointmentsToday();
    bool removeAppointmentById(const int id);
    bool removeAllAppointments();

    QList<Contact> getAllContacts();
    bool removeAllContacts();
    bool removeContactById(const int id);
    QList<Contact> findContactsByLastName(QString lastname);




};

#endif // DBMANAGER_H
