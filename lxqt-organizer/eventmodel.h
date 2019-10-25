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

#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QDate>
#include <QDebug>
#include "event.h"

class EventModel : public QAbstractListModel
{
     Q_OBJECT
public:
    EventModel(QObject* parent = nullptr);
    ~EventModel() override;
    enum EventRoles {
        LocationRole = Qt::UserRole,
    };

    void addEvent(Event *appointment);
    Event *getEvent(int index);
    void clearAllEvents();
    void removeEvent(int idx);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<Event*> eventList;
};

#endif // EVENTMODEL_H
