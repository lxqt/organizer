/***************************************************************************
 *   Author Alan Crispin aka. crispina                 *
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

#ifndef DIALOGCONTACT_H
#define DIALOGCONTACT_H

#include <QDialog>
#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include "contact.h"

namespace Ui {
class DialogContact;
}

class DialogContact : public QDialog
{
    Q_OBJECT

public:
    explicit DialogContact(QWidget *parent = nullptr);

   explicit DialogContact(Contact *theContact=nullptr, QWidget *parent = nullptr);
    ~DialogContact();

    QString firstName="";
    QString middleNames="";
    QString lastName="";
    QString email="";
    QDate birthDate;
    int birthDateId=0;

    QString getFirstName();
    QString getMiddleNames();
    QString getLastName();
    QString getEmail();
    QDate getBirthDate();
    int getAddToCalendar();

    //Contact details
    QString street="";
    QString district="";
    QString city="";
    QString county="";
    QString postcode="";
    QString country="";
    QString phoneNumber; //could have + code
    bool deleteRequested=false;
    int addToCalendar=0;

    QString getStreet();
    QString getDistrict();
    QString getCity();
    QString getCounty();
    QString getPostcode();
    QString getCountry();
    QString getPhoneNumber();
    bool getDeleteRequested();



private slots:
        void accept();
        void on_checkBoxBirthdayUnknown_stateChanged(int arg1);

        void on_dateEditBirthDate_userDateChanged(const QDate &date);

        void on_checkBoxAddBirthdayToCal_stateChanged(int arg1);

        void on_checkBoxDelete_stateChanged(int arg1);

private:
    Ui::DialogContact *ui;
};

#endif // DIALOGCONTACT_H
