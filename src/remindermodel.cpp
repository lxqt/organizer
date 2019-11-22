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
    this->modelReminderList= QList<Reminder>();

}

ReminderModel::ReminderModel(const QList<Reminder> &reminderList, QObject *parent)
{
    Q_UNUSED(parent)
   this->modelReminderList=reminderList;
}

void ReminderModel::addReminder(Reminder &reminder)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->modelReminderList.append(reminder);
    endInsertRows();
}

Reminder ReminderModel::getAReminder(int index)
{
    Reminder r = modelReminderList.value(index);
    return r;
}

void ReminderModel::clearAllReminders()
{
    beginResetModel();
    modelReminderList.clear();
    endResetModel();
}

void ReminderModel::removeReminder(int idx)
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

        const Reminder& reminder = modelReminderList.at(index.row());


        return reminder.m_details;
    }

        return QVariant();
}
