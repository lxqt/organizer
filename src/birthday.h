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

#ifndef BIRTHDAY_H
#define BIRTHDAY_H

#include <QString>
struct Birthday {

    Birthday(int id=0,
                const QString& name= QString(),
                const QString& location= QString(),
                const QString& description= QString(),
                const QString& birthDate=QString(),
                int addToCalendar=1
                ):
        m_id(id),
        m_name(name),
        m_location(location),
        m_description(description),
        m_birthDate(birthDate),
        m_addToCalendar(addToCalendar)
    {
    }
   int m_id;
   QString m_name;
   QString m_location;
   QString m_description;
   QString m_birthDate;
   int m_addToCalendar;
};



#endif // BIRTHDAY_H
