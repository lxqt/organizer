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
#ifndef DIALOGADDCONTACT_H
#define DIALOGADDCONTACT_H

#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QString>


namespace Ui {
class DialogAddContact;
}

class DialogAddContact : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddContact(QWidget *parent = nullptr, QDate *theBirthdayDate=nullptr);
    ~DialogAddContact();

    QString firstName="";
    QString lastName="";
    QString email="";
    QDate bornDate;

    QString getFirstName();
    QString getLastName();
    QString getEmail();
    QDate getBirthday();

    bool addToBirthdayAppointment=true;
    bool getAddBirthdayAppointment();


    //Reminder
    QDate reminderDate;
    QTime reminderTime;
    int reminderRequested=0;

    QDate getReminderDate();
    QTime getReminderTime();
    int getReminderRequested();    

    //Contact details
    QString street="";
    QString city="";
    QString county="";
    QString postcode="";
    QString country="";
    QString phoneNumber; //could have + code

    QString getStreet();
    QString getCity();
    QString getCounty();
    QString getPostcode();
    QString getCountry();
    QString getPhoneNumber();



private slots:
void accept();

void on_dateEditBirthday_userDateChanged(const QDate &date);

void on_dateEditReminder_userDateChanged(const QDate &date);

void on_timeEditReminder_userTimeChanged(const QTime &time);

void on_checkBoxReminder_stateChanged(int arg1);

void on_checkBoxContactDetails_stateChanged(int arg1);

void on_checkBoxAddToCalendar_stateChanged(int arg1);

private:
    Ui::DialogAddContact *ui;
};

#endif // DIALOGADDCONTACT_H
