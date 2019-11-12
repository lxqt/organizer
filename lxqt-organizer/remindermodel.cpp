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
#include "remindermodel.h"


ReminderModel::ReminderModel(QObject *parent)
{
    Q_UNUSED(parent)
    this->modelReminderList= QList<Appointment>();

}

ReminderModel::ReminderModel(const QList<Appointment> &appointmentList, QObject *parent)
{
    Q_UNUSED(parent)
   this->modelReminderList=appointmentList;
}

void ReminderModel::addReminderAppointment(Appointment &appointment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->modelReminderList.append(appointment);
    endInsertRows();
}

Appointment ReminderModel::getAReminderAppointment(int index)
{
    Appointment a = modelReminderList.value(index);
    return a;
}

void ReminderModel::clearAllReminderAppointments()
{
    beginResetModel();
    modelReminderList.clear();
    endResetModel();
}

void ReminderModel::removeReminderAppointment(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelReminderList.removeAt(idx);
    endRemoveRows();
}

int ReminderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelReminderList.size();
}

QVariant ReminderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelReminderList.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {

        const Appointment& appointment = modelReminderList.at(index.row());

        QString startTimestamp =appointment.m_appointmentStartTimestamp;
        QString endTimestamp =appointment.m_appointmentEndTimestamp;
        QDateTime startdatetime =QDateTime::fromString(startTimestamp);
        QDateTime enddatetime=QDateTime::fromString(endTimestamp);

        QDate startDate =startdatetime.date();
        QTime startTime =startdatetime.time();
        //QDate endDate= enddatetime.date();
        QTime endTime = enddatetime.time();

        int appointmentDay =startDate.day();
        int appointmentMonth =startDate.month();
        int appointmentYear =startDate.year();
        int appointmentStartHour = startTime.hour();
        int appointmentStartMin=startTime.minute();
        int appointmentEndHour=endTime.hour();
        int appointmentEndMin=endTime.minute();


        QString dateStr="("+QString::number(appointmentDay)+"/"
                +QString::number(appointmentMonth)+"/"
                +QString::number(appointmentYear)+")";
        QString minutesStartStr = QString("%1").arg(appointmentStartMin, 2, 10, QChar('0'));
        QString startTimeStr=QString::number(appointmentStartHour,'f',0)
                +":"+minutesStartStr;
        QString minutesEndStr = QString("%1").arg(appointmentEndMin, 2, 10, QChar('0'));
        QString endTimeStr=QString::number(appointmentEndHour,'f',0)+
                ":"+minutesEndStr;
        QString reminderDetails=appointment.m_title
                +" ("+appointment.m_location
                        + ") "+dateStr+" ("
                 +startTimeStr+"->"+endTimeStr+")";

        return reminderDetails;
    }

        return QVariant();
}
