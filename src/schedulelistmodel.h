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

#ifndef SCHEDULELISTMODEL_H
#define SCHEDULELISTMODEL_H

#include <QAbstractListModel>
#include <QTime>
#include "appointment.h"
//#include "dialogupcomingschedule.h"

class ScheduleListModel: public QAbstractListModel
{
public:
    ScheduleListModel(QObject* parent = nullptr);
    ScheduleListModel(const QList<Appointment>& appointmentList,
                        QObject *parent = nullptr);

    void addAppointment(Appointment &appointment);
    void updateAppointment(Appointment &appointment, int index);
    Appointment getAppointment(int index);
    void clearAllAppointment();
    void removeAppointment(int idx);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data (const QModelIndex & index,
                   int role = Qt::DisplayRole) const override;
private:
    QList<Appointment> modelAppointmentList;





};

#endif // SCHEDULELISTMODEL_H
