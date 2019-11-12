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

#ifndef DBUSSESSIONMESSAGE_H
#define DBUSSESSIONMESSAGE_H

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QObject>
#include <QTimer>
#include <QDebug>

#define SERVICE_FREEDESKTOP "org.freedesktop.Notifications"
#define PATH_FREEDESKTOP "/org/freedesktop/Notifications"
#define INTERFACE_FREEDESKTOP "org.freedesktop.Notifications"

struct MessageData {
    QString header;
    QString message;

    MessageData(QString header, QString message)
    {
        this->header = header;
        this->message = message;
    }
};

class QDBusInterface; //declare the interface

class DbusSessionMessage: public QObject
{
    Q_OBJECT


public:
    explicit DbusSessionMessage(QObject *parent = nullptr);
    void displayMessage(const MessageData& message);

private:
    QList<QVariant> prepareDbusMessage(const MessageData& message);
    QDBusInterface *dbusNotifier;
    int messageDuration;
    QString theIconPath;
};

#endif // DBUSSESSIONMESSAGE_H
