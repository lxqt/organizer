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
#include "dialogaddcontact.h"
#include "ui_dialogaddcontact.h"

DialogAddContact::DialogAddContact(QWidget *parent, QDate *theSelectedDate) :
    QDialog(parent),
    ui(new Ui::DialogAddContact)
{
    ui->setupUi(this);
    setWindowTitle("New Contact");   
    ui->dateEditReminder->setDate(*theSelectedDate);
    reminderDate=*theSelectedDate;
    //reminderTime=QTime::currentTime();
    reminderTime=ui->timeEditReminder->time();
    //ui->timeEditReminder->setTime(reminderTime);
    ui->checkBoxReminder->setCheckState(Qt::Unchecked);
    ui->dateEditReminder->setDisabled(true);
    ui->timeEditReminder->setDisabled(true);
    reminderRequested=0;
    ui->checkBoxContactDetails->setCheckState(Qt::Unchecked);
    ui->lineEditStreet->setDisabled(true);
    ui->lineEditCity->setDisabled(true);
    ui->lineEditCounty->setDisabled(true);
    ui->lineEditPostcode->setDisabled(true);
    ui->lineEditCountry->setDisabled(true);
    ui->lineEditPhone->setDisabled(true);

}

DialogAddContact::~DialogAddContact()
{
    delete ui;
}

QString DialogAddContact::getFirstName()
{
    firstName = ui->lineEditFirstName->text();
    return firstName;
}

QString DialogAddContact::getLastName()
{
    lastName=ui->lineEditLastName->text();
    return lastName;
}


QString DialogAddContact::getEmail()
{
    email=ui->lineEditEmail->text();
    return email;
}

QDate DialogAddContact::getBirthday()
{
    return this->bornDate;
}

bool DialogAddContact::getAddBirthdayAppointment()
{
    return addToBirthdayAppointment;
}

QDate DialogAddContact::getReminderDate()
{
    return this->reminderDate;
}

QTime DialogAddContact::getReminderTime()
{
    return this->reminderTime;
}

int DialogAddContact::getReminderRequested()
{
    return this->reminderRequested;
}



QString DialogAddContact::getStreet()
{
    this->street=ui->lineEditStreet->text();
    return this->street;
}

QString DialogAddContact::getCity()
{
    this->city =ui->lineEditCity->text();
    return this->city;
}

QString DialogAddContact::getCounty()
{
    this->county=ui->lineEditCounty->text();
    return this->county;

}

QString DialogAddContact::getPostcode()
{
    this->postcode=ui->lineEditPostcode->text();
    return this->postcode;
}

QString DialogAddContact::getCountry()
{
    this->country=ui->lineEditCountry->text();
    return this->country;
}

QString DialogAddContact::getPhoneNumber()
{   
    this->phoneNumber=ui->lineEditPhone->text();
    return this->phoneNumber;
}

void DialogAddContact::accept()
{
    if (this->getFirstName().isEmpty() || this->getLastName().isEmpty())
    {
        QMessageBox::information(this, tr("Empty Details"),
                  tr("Must enter first and last name"));
                return;

    }
    else {
        //qDebug()<<"Success both fields completed by user";
        QDialog::accept();
    }
}


void DialogAddContact::on_dateEditBirthday_userDateChanged(const QDate &date)
{
    this->bornDate=date;   
}

void DialogAddContact::on_dateEditReminder_userDateChanged(const QDate &date)
{
    this->reminderDate=date;
}

void DialogAddContact::on_timeEditReminder_userTimeChanged(const QTime &time)
{
    this->reminderTime=time;
}

void DialogAddContact::on_checkBoxReminder_stateChanged(int arg1)
{
    if (arg1==Qt::Checked)
    {
        //enable reminder date time edit boxes
        ui->dateEditReminder->setDisabled(false);
        ui->timeEditReminder->setDisabled(false);
        reminderRequested=1;
    }
    else if (arg1==Qt::Unchecked) {
        //disable reminder date and time edit boxes
        ui->dateEditReminder->setDisabled(true);
        ui->timeEditReminder->setDisabled(true);
        reminderRequested=0;
    }
}

void DialogAddContact::on_checkBoxContactDetails_stateChanged(int arg1)
{
    if (arg1==Qt::Checked)
        {
            //enable
            ui->lineEditStreet->setDisabled(false);
            ui->lineEditCity->setDisabled(false);
            ui->lineEditCounty->setDisabled(false);
            ui->lineEditPostcode->setDisabled(false);
            ui->lineEditCountry->setDisabled(false);
            ui->lineEditPhone->setDisabled(false);
        }
        else if (arg1==Qt::Unchecked) {
            //disable
            ui->lineEditStreet->setDisabled(true);
            ui->lineEditCity->setDisabled(true);
            ui->lineEditCounty->setDisabled(true);
            ui->lineEditPostcode->setDisabled(true);
            ui->lineEditCountry->setDisabled(true);
            ui->lineEditPhone->setDisabled(true);
        }
}

void DialogAddContact::on_checkBoxAddToCalendar_stateChanged(int arg1)
{
    if (arg1==Qt::Checked)
    {
        addToBirthdayAppointment=true;
    }
    else if (arg1==Qt::Unchecked) {
        addToBirthdayAppointment=false;
    }

}
