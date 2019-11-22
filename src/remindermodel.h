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
#ifndef REMINDERMODEL_H
#define REMINDERMODEL_H

#include <QColor>
#include <QList>
#include <QDate>
#include <QDebug>
#include "reminder.h"
#include <QAbstractListModel>

class ReminderModel: public QAbstractListModel
{
public:
    ReminderModel(QObject* parent = nullptr);

    ReminderModel(const QList<Reminder>& reminderList,
                                  QObject *parent = nullptr);

    void addReminder(Reminder &reminder);
    Reminder getAReminder(int index);
    void clearAllReminders();
    void removeReminder(int idx);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data (const QModelIndex & index,
                           int role = Qt::DisplayRole) const override;


private:
    QList<Reminder> modelReminderList;



};

#endif // REMINDERMODEL_H
