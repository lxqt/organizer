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

#include "systemtimer.h"

SystemTimer::SystemTimer()
{    
    timer = new QTimer(this);
    // setup signal and slot mechanism
    connect(timer, &QTimer::timeout, this, &SystemTimer::SystemTimerSlot);
    timer->start(10000); //ms
}

void SystemTimer::checkReminders(int currentHour)
{    
    currentDate = QDate::currentDate();
    QList<Event> list =MainWindow().getAllAppointments();
    foreach(Event a, list){
       int reminderHour =a.reminderTime;
       QDate reminderDate = QDate(a.reminderYear,a.reminderMonth,a.reminderDay);

       if (reminderDate==currentDate)
       {
          // qDebug()<<"Current and reminder dates equal";

           if(currentHour==reminderHour){
               qDebug()<<"Date and time match: Show Reminder Alarm!";
               if(MainWindow().notificationsFlag){
               QString h="Reminder Alarm: "+a.title +" ("+a.location+")";
               QDate tmp =QDate(a.year,a.month,a.day);
               QString m="Date: "+tmp.toString()+ " Starts at "+QString::number(a.startTime)+":00";
               MessageData *msg = new MessageData(h,m);
               DbusSessionMessage *dbus = new DbusSessionMessage(this);
               dbus->displayMessage(*msg);
               }
           }
       }
       else {
           //qDebug()<<"Dates not equal";
       }

    }
}

void SystemTimer::SystemTimerSlot()
{
    //qDebug()<<"Tick tock...";
    currentTime =QTime::currentTime();
    int hour = currentTime.hour();
    int minute = currentTime.minute();
    for (int i=0; i<24; i++)
    {
        //Check 24 hour clock
        if (hour ==i){           
            if(minute ==0)
            {                
                checkReminders(i);
            }
        }
    }
}

