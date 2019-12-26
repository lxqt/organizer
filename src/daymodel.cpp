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


#include "daymodel.h"

DayModel::DayModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    Q_UNUSED(parent)
}

DayModel::DayModel(const QList<Appointment> &appointmentList, QObject *parent)
    : QAbstractTableModel(parent)
{
    Q_UNUSED(parent)
    this->modelAppointmentList=appointmentList;
}

void DayModel::AddAppointment(Appointment &appointment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    modelAppointmentList.append(appointment);
    endInsertRows();
}

Appointment DayModel::getAppointment(int index)
{
    Appointment a = modelAppointmentList.value(index);
    return a;
}

void DayModel::clearAllAppointments()
{
    beginResetModel();
    modelAppointmentList.clear();
    endResetModel();
}

void DayModel::removeAppointment(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelAppointmentList.removeAt(idx);
    endRemoveRows();

}

int DayModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelAppointmentList.size();

}

int DayModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;

}

QVariant DayModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
           return QVariant();
       if (index.row() >= modelAppointmentList.size() || index.row() < 0)
           return QVariant();

       const Appointment& a = modelAppointmentList.at(index.row());

       if (role == Qt::DisplayRole) {

           QTime starts =QTime::fromString(a.m_startTime);

           switch (index.column()) {
           case 0:
               return a.m_id;
           case 1:
               return starts.toString("hh:mm");
           case 2:
               return a.m_title;
           case 3:
               return a.m_description;
           case 4:
               return a.m_location;
           default:
               return QVariant();
           }
       }

       return QVariant();

}
QVariant DayModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Id"; //AppointmentId
        case 1:
            return "Starts";
        case 2:
            return "Title";
        case 3:
            return "Details";
        case 4:
            return "Location";
        default:
            return QVariant();
        }
    }
   return section + 1;
}




