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


#include "reminderslistmodel.h"

remindersListModel::remindersListModel(QObject *parent)
{
    Q_UNUSED(parent)
    this->modelReminderList= QList<Reminder>();

}

remindersListModel::remindersListModel(const QList<Reminder> &reminderList, QObject *parent)
{
    Q_UNUSED(parent)
   this->modelReminderList=reminderList;
}



void remindersListModel::addReminder(Reminder &reminder)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->modelReminderList.append(reminder);
    endInsertRows();
}

void remindersListModel::updateReminder(Reminder &reminder, int index)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
        Reminder r = modelReminderList.value(index);
        r.m_details=reminder.m_details;
        r.m_reminderDate=reminder.m_reminderDate;
        r.m_reminderTime=reminder.m_reminderTime;
        modelReminderList.removeAt(index);
        modelReminderList.insert(index,r);
       endInsertRows();
}

Reminder remindersListModel::getReminder(int index)
{
    Reminder r = modelReminderList.value(index);
    return r;
}

void remindersListModel::clearAllReminders()
{
    beginResetModel();
    modelReminderList.clear();
    endResetModel();
}

void remindersListModel::removeReminder(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelReminderList.removeAt(idx);
    endRemoveRows();
}

int remindersListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelReminderList.size();
}

QVariant remindersListModel::data(const QModelIndex &index, int role) const
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








