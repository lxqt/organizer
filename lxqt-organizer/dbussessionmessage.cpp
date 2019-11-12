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

#include "dbussessionmessage.h"

DbusSessionMessage::DbusSessionMessage(QObject *parent) : QObject(parent)
{
   //Use the default session bus for desktop message notifications
    dbusNotifier = new QDBusInterface(QStringLiteral(SERVICE_FREEDESKTOP), QStringLiteral(PATH_FREEDESKTOP), QStringLiteral(INTERFACE_FREEDESKTOP),
                                      QDBusConnection::sessionBus(), this);
       if (dbusNotifier->lastError().type() != QDBusError::NoError) {
           qDebug() << "Unable to create QDBus interface.";
           return;
       }
       //qDebug() << "QDBus interface successfully created (it works!)";

       this->messageDuration=2000;
       this->theIconPath="";
}

void DbusSessionMessage::displayMessage(const MessageData &message)
{
    QList<QVariant> n = prepareDbusMessage(message);
    if (!n.isEmpty()) {
        QDBusReply<uint> reply = dbusNotifier->callWithArgumentList(QDBus::Block,QStringLiteral("Notify"),n);
    }
    deleteLater();
}

QList<QVariant> DbusSessionMessage::prepareDbusMessage(const MessageData &message)
{
    QList<QVariant> arguments;
    arguments << QLatin1Literal("LXQt Organizer");
    arguments << QVariant(QVariant::UInt);  //the ID
    arguments << theIconPath; //path to icon (null)
    arguments << message.header; //message header
    arguments << message.message; //the message
    arguments << QStringList();
    arguments << QVariantMap();
    arguments << messageDuration;
    return arguments;
}


