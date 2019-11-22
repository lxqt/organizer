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
    return 9; //9 columns (cases below)
}

QVariant AppointmentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelAppointmentList.size() || index.row() < 0)
        return QVariant();


    if (role == Qt::DisplayRole) {
        const Appointment& a = modelAppointmentList.at(index.row());

        QDate appointmentDate=QDate::fromString(a.m_appointmentDate);
        QTime startTime=QTime::fromString(a.m_appointmentStartTime);
        QTime endTime=QTime::fromString(a.m_appointmentEndTime);


        int appointmentDay =appointmentDate.day();
        int appointmentMonth =appointmentDate.month();
        int appointmentYear =appointmentDate.year();
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
        switch (index.column()) {
        case 0:
            return a.m_id; //hidden (nneded for appointment removal)
        case 1:
            return appointmentStartHour; //going to sort on start hour column 1 -hide
        case 2:
            return appointmentStartMin; //going to sort on start min  column 2 -hide
        case 3:
            return dateStr;
        case 4:
            return startTimeStr; //format to 2 decimal place
        case 5:
            return endTimeStr;
        case 6:
            return a.m_title;
        case 7:
            return a.m_location;
        case 8:
            return a.m_description;

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
            return "Id";
        case 1:
            return "Hour";
        case 2:
            return "Min";
        case 3:
            return "Date";
        case 4:
            return "Starts";
        case 5:
            return "Ends";
        case 6:
            return "Title";
        case 7:
            return "Location";
        case 8:
            return "Description";
        default:
            return QVariant();
        }
    }
    return section + 1;
}




