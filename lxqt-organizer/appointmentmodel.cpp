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


#include "appointmentmodel.h"

AppointmentModel::AppointmentModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

AppointmentModel::AppointmentModel(const QList<Appointment> &appointmentList, QObject *parent)
{
     Q_UNUSED(parent)
    this->modelAppointmentList=appointmentList;
}

void AppointmentModel::addAppointment(Appointment &appointment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    modelAppointmentList.append(appointment);
    endInsertRows();
}

Appointment AppointmentModel::getAppointment(int index)
{
    Appointment a = modelAppointmentList.value(index);
    return a;
}

void AppointmentModel::clearAllAppointments()
{
    beginResetModel();
    modelAppointmentList.clear();
    endResetModel();
}

void AppointmentModel::removeAppointment(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelAppointmentList.removeAt(idx);
    endRemoveRows();
}

int AppointmentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelAppointmentList.size();
}

int AppointmentModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7; //7 columns (0-6 cases below)
}

QVariant AppointmentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelAppointmentList.size() || index.row() < 0)
        return QVariant();


    if (role == Qt::DisplayRole) {
        const Appointment& appointment = modelAppointmentList.at(index.row());
        QString dateStr="("+QString::number(appointment.m_day)+"/"
                +QString::number(appointment.m_month)+"/"
                +QString::number(appointment.m_year)+")";
        switch (index.column()) {
        case 0:
            return appointment.m_startTime; //going to sort on start time column zero -hide
        case 1:
            return appointment.m_id; //get the database id (needed for appointment removal)-hide
        case 2:
            return dateStr;
        case 3:
            return QString::number(appointment.m_startTime, 'f', 2); //format to 2 decimal place
        case 4:
            return QString::number(appointment.m_endTime, 'f', 2);
        case 5:
            return appointment.m_title;
        case 6:
            return appointment.m_location;

        default:
            return QVariant();
        }
    }
    else if (role == Qt::TextAlignmentRole){
        if (index.column() == 3 || index.column()==4) //start and end times
            return Qt::AlignCenter;
        else
            return Qt::AlignLeft;
    }
    return QVariant();
}

QVariant AppointmentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
               return QVariant();

           if (orientation == Qt::Horizontal) {
               switch (section) {
               case 0:
                   return "Time";
               case 1:
                   return "Id";
               case 2:
                   return "Date";
               case 3:
                   return "Starts";
               case 4:
                   return "Ends";
               case 5:
                   return "Title";
               case 6:
                   return "Location";
               default:
                   return QVariant();
               }
           }
           return section + 1;
}




