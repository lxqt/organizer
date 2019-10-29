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


class DbManager
{
public:
    DbManager();
    QSqlDatabase db;
    bool isOpen() const;
    void openDatabase();
    void createDatabaseTable();

    bool removeAllAppointments();
    bool removeAppointmentById(const int id);

    bool addAppointment(QString &title, QString &location,
                            int &day, int &month, int &year,
                            int &starttime, int &endtime,
                            int &remday, int &remmonth, int &remyear, int&remtime);
    QList<Appointment> getAllAppointments();
    QList<Appointment> getAppointmentsToday();
    QList<Appointment> getAppointmentsOnDate(QDate *diaryDate);
};

#endif // DBMANAGER_H
