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

#include "eventmodel.h"

EventModel::EventModel(QObject* parent):
    QAbstractListModel(parent),
    eventList()
{

}

EventModel::~EventModel()
{
    qDeleteAll(eventList);
}

void EventModel::addEvent(Event *appointment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    eventList.append(appointment);
    endInsertRows();
}

Event *EventModel::getEvent(int index)
{
    Event *a =eventList.value(index);
    return a;
}

void EventModel::clearAllEvents()
{
    beginResetModel();
    eventList.clear();
    endResetModel();
}

void EventModel::removeEvent(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    eventList.removeAt(idx);
    endRemoveRows();
}

int EventModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return eventList.size();
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    const Event& appointment = *eventList.at(index.row());

        int day = appointment.day;
        int month =appointment.month;
        int year =appointment.year;
        int starts =appointment.startTime;
        QDate *appointmentDate = new QDate(year,month,day);

        //details determines what is displayed by display role (default)
        QString details=appointment.title+" ("+appointment.location
                + ") "+appointmentDate->toString()+" "+QString::number(starts)+":00";
        switch (role) {
        case Qt::DisplayRole:
            return details;

        case EventRoles::LocationRole:
            return appointment.location;
        //more roles to be added later (crispina)
        default:
            return QVariant();
            break; //just in case

        }
}
