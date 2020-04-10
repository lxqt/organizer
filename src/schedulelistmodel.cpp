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

#include "schedulelistmodel.h"

ScheduleListModel::ScheduleListModel(QObject *parent)
{
    Q_UNUSED(parent)
    this->modelAppointmentList= QList<Appointment>();

}

ScheduleListModel::ScheduleListModel(const QList<Appointment> &appointmentList, QObject *parent)
{
    Q_UNUSED(parent)
    this->modelAppointmentList=appointmentList;

}

void ScheduleListModel::addAppointment(Appointment &appointment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->modelAppointmentList.append(appointment);
    endInsertRows();

}

void ScheduleListModel::updateAppointment(Appointment &appointment, int index)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Appointment a = modelAppointmentList.value(index);
    a.m_title= appointment.m_title;
    a.m_location=appointment.m_location;
    a.m_description=appointment.m_description;
    a.m_startTime=appointment.m_startTime;
    a.m_endTime=appointment.m_endTime;

    a.m_isFullDay=appointment.m_isFullDay;
    modelAppointmentList.removeAt(index);
    modelAppointmentList.insert(index,a);
    endInsertRows();
}

Appointment ScheduleListModel::getAppointment(int index)
{
    Appointment a = modelAppointmentList.value(index);
    return a;
}

void ScheduleListModel::clearAllAppointment()
{
    beginResetModel();
    modelAppointmentList.clear();
    endResetModel();
}

void ScheduleListModel::removeAppointment(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelAppointmentList.removeAt(idx);
    endRemoveRows();
}

int ScheduleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelAppointmentList.size();

}

QVariant ScheduleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelAppointmentList.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {

        const Appointment& appointment = modelAppointmentList.at(index.row());
        QString details=QStringLiteral("default");
        //More to do..
        QTime starts =QTime::fromString(appointment.m_startTime);
        QTime ends =QTime::fromString(appointment.m_endTime);

        if (appointment.m_isFullDay==1)
        {
            details =QLatin1String("Title: ")+appointment.m_title
                    +QLatin1String("\nDate: ")+appointment.m_date
                    +QLatin1String("\nLocation: ")+appointment.m_location
                    +QStringLiteral("\nAll Day Event") +QLatin1String("\nNotes: ")+appointment.m_description
                    +QLatin1String("\n----------------------");

        }
        else {
            details =QLatin1String("Title: ")+appointment.m_title
                    +QLatin1String("\nDate: ")+appointment.m_date
                    +QLatin1String("\nLocation: ")+appointment.m_location
                    + QStringLiteral("\nStart Time: ")
                    +starts.toString(QStringLiteral("hh:mm"))+QStringLiteral(" End Time: ")+ends.toString(QStringLiteral("hh:mm"))
                    +QLatin1String("\nNotes: ")+appointment.m_description
                    +QLatin1String("\n----------------------");
        }

        return details;
    }

    return QVariant();

}
