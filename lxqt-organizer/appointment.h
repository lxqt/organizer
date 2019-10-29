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

#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QString>
struct Appointment {

    Appointment(int id=0,
                const QString& title= QString(),
                const QString& location= QString(),
                int day=0,
                int month=0,
                int year=0,
                int startTime=0,
                int endTime=0,
                int reminderDay=0,
                int reminderMonth=0,
                int reminderYear=0,
                int reminderTime=0 ):
        m_id(id),
        m_title(title),
        m_location(location),
        m_day(day),
        m_month(month),
        m_year(year),
        m_startTime(startTime),
        m_endTime(endTime),
        m_reminderDay(reminderDay),
        m_reminderMonth(reminderMonth),
        m_reminderYear(reminderYear),
        m_reminderTime(reminderTime)
    {
    }
   int m_id;
   QString m_title;
   QString m_location;
   int m_day;
   int m_month;
   int m_year;
   int m_startTime;
   int m_endTime;
   int m_reminderDay;
   int m_reminderMonth;
   int m_reminderYear;
   int m_reminderTime;
};


#endif // APPOINTMENT_H
