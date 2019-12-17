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

#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
struct Contact {

    Contact(int id=0,
            const QString& firstname= QString(),
            const QString& midnames= QString(),
            const QString& lastname= QString(),
            const QString& email= QString(),
            const QString& street= QString(),
            const QString& district= QString(),
            const QString& city= QString(),
            const QString& county= QString(),
            const QString& postcode= QString(),
            const QString& country= QString(),
            const QString& telephone= QString(),           
            const QString& birthdate=QString(),
            int addBirthday=0,
            int birthdayappointmentid=0
            ):
        m_id(id),
        m_firstname(firstname),
        m_midnames(midnames),
        m_lastname(lastname),
        m_email(email),
        m_street(street),
        m_district(district),
        m_city(city),
        m_county(county),
        m_postcode(postcode),
        m_country(country),
        m_telephone(telephone),        
        m_birthdate(birthdate),
        m_addbirthday(addBirthday),
        m_birthdayAppointmentId(birthdayappointmentid)
    {
    }
    int m_id;
    QString m_firstname;
    QString m_midnames;
    QString m_lastname;
    QString m_email;
    QString m_street;
    QString m_district;
    QString m_city;
    QString m_county;
    QString m_postcode;
    QString m_country;
    QString m_telephone;    
    QString m_birthdate;
    int m_addbirthday;
    int m_birthdayAppointmentId;
};


#endif // CONTACT_H
