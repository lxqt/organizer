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

#include "dialogcontact.h"
#include "ui_dialogcontact.h"

DialogContact::DialogContact(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogContact)
{
    ui->setupUi(this);
    //New Contact
    //setWindowTitle(QStringLiteral("New Contact"));
    ui->checkBoxDelete->hide();
    this->birthDate=QDate(); //null

    setLabelDescriptions();


    ui->checkBoxBirthdayUnknown->setText(QStringLiteral("Birthday Unknown"));
    ui->checkBoxAddBirthdayToCal->setText(QStringLiteral("Add To Calendar"));
    ui->checkBoxBirthdayUnknown->setCheckState(Qt::Unchecked);
    ui->dateEditBirthDate->setDate(QDate::currentDate());

}

DialogContact::DialogContact(Contact *theContact, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogContact)
{
    ui->setupUi(this);
    //Update Contact
    //setWindowTitle(QStringLiteral("Update Contact"));
    ui->checkBoxDelete->show();
    setLabelDescriptions();

    ui->lineEditFirstName->setText(theContact->m_firstname);
    ui->lineEditMiddleNames->setText(theContact->m_midnames);
    ui->lineEditLastName->setText(theContact->m_lastname);
    ui->lineEditEmail->setText(theContact->m_email);
    ui->lineEditStreet->setText(theContact->m_street);
    ui->lineEditDistrict->setText(theContact->m_district);
    ui->lineEditCity->setText(theContact->m_city);
    ui->lineEditCounty->setText(theContact->m_county);
    ui->lineEditPostcode->setText(theContact->m_postcode);
    ui->lineEditCountry->setText(theContact->m_country);
    ui->lineEditTelephone->setText(theContact->m_telephone);


    this->birthDate=QDate::fromString(theContact->m_birthdate);

    //qDebug()<<"Contact Dialog: birthdate = "<<birthDate.toString();
    if (!birthDate.isNull())
    {
        ui->dateEditBirthDate->setDate(this->birthDate);
    }
    else {

        //qDebug()<<"Set checked state...";
        ui->dateEditBirthDate->clear();
        ui->checkBoxBirthdayUnknown->setCheckState(Qt::Checked);
    }

    this->addToCalendar=theContact->m_addToCalendar;

    if (addToCalendar==1)
    {
        ui->checkBoxAddBirthdayToCal->setCheckState(Qt::Checked);
    }
    else {
        ui->checkBoxAddBirthdayToCal->setCheckState(Qt::Unchecked);
    }

}


DialogContact::~DialogContact()
{
    delete ui;
}

QString DialogContact::getFirstName()
{
    firstName = ui->lineEditFirstName->text();
    return firstName;
}

QString DialogContact::getMiddleNames()
{
    middleNames=ui->lineEditMiddleNames->text();
    return middleNames;
}

QString DialogContact::getLastName()
{
    lastName=ui->lineEditLastName->text();
    return lastName;
}

QString DialogContact::getEmail()
{
    email=ui->lineEditEmail->text();
    return email;
}

QDate DialogContact::getBirthDate()
{
    return this->birthDate;
}

int DialogContact::getAddToCalendar()
{
    return this->addToCalendar;
}

QString DialogContact::getStreet()
{
    this->street=ui->lineEditStreet->text();
    return this->street;
}

QString DialogContact::getDistrict()
{
    this->district=ui->lineEditDistrict->text();
    return this->district;
}

QString DialogContact::getCity()
{
    this->city =ui->lineEditCity->text();
    return this->city;
}

QString DialogContact::getCounty()
{
    this->county=ui->lineEditCounty->text();
    return this->county;
}

QString DialogContact::getPostcode()
{
    this->postcode=ui->lineEditPostcode->text();
    return this->postcode;

}

QString DialogContact::getCountry()
{
    this->country=ui->lineEditCountry->text();
    return this->country;
}

QString DialogContact::getPhoneNumber()
{
    this->phoneNumber=ui->lineEditTelephone->text();
    return this->phoneNumber;
}

bool DialogContact::getDeleteRequested()
{
    return this->deleteRequested;
}

void DialogContact::setDialogTitleTranslation(QString translation)
{
    t_dialog_title=translation;
}

void DialogContact::setFirstNameTranslation(QString translation)
{
    t_first_name=translation;
}

void DialogContact::setMidNameTranslation(QString translation)
{
    t_mid_name=translation;
}

void DialogContact::setLastNameTranslation(QString translation)
{
    t_last_name=translation;
}

void DialogContact::setEmailTranslation(QString translation)
{
    t_email=translation;
}

void DialogContact::setStreetTranslation(QString translation)
{
    t_street=translation;
}

void DialogContact::setDistrictTranslation(QString translation)
{
    t_district=translation;
}

void DialogContact::setCityTranslation(QString translation)
{
    t_city=translation;
}

void DialogContact::setCountyTranslation(QString translation)
{
    t_county=translation;
}

void DialogContact::setPostcodeTranslation(QString translation)
{
    t_postcode=translation;
}

void DialogContact::setCountryTranslation(QString translation)
{
    t_country=translation;
}

void DialogContact::setTelephoneTranslation(QString translation)
{
    t_telephone=translation;
}

void DialogContact::setBornOnTranslation(QString translation)
{
    t_born_on=translation;
}

void DialogContact::setBirthdayUnknownTranslation(QString translation)
{
    t_birthday_unknown=translation;
}

void DialogContact::setAddToCalendarTranslation(QString translation)
{
    t_add_to_calendar=translation;
}

void DialogContact::setDeleteTranslation(QString translation)
{
    t_delete=translation;
}

void DialogContact::setLabelDescriptions()
{
    setWindowTitle(t_dialog_title);
    ui->labelFirstName->setText(t_first_name);
    ui->labelMidNames->setText(t_mid_name);
    ui->labelLastName->setText(t_last_name);
    ui->labelEmail->setText(t_email);
    ui->labelStreet->setText(t_street);
    ui->labelDistrict->setText(t_district);
    ui->labelCity->setText(t_city);
    ui->labelCounty->setText(t_county);
    ui->labelPostcode->setText(t_postcode);
    ui->labelCountry->setText(t_country);
    ui->labelPhone->setText(t_telephone);

    ui->checkBoxBirthdayUnknown->setText(t_birthday_unknown);
    ui->labelBornOn->setText(t_born_on);
    ui->checkBoxAddBirthdayToCal->setText(t_add_to_calendar);
    ui->checkBoxDelete->setText(t_delete);

}

void DialogContact::accept()
{
    if (this->getFirstName().isEmpty() || this->getLastName().isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("Empty Details"),
                                 QStringLiteral("Must enter first and last name"));
        return;
    }
    else {
        //qDebug()<<"Success both fields completed by user";
        QDialog::accept();
    }
}

void DialogContact::on_checkBoxBirthdayUnknown_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        ui->dateEditBirthDate->setEnabled(true);
        ui->checkBoxAddBirthdayToCal->setEnabled(true);
        ui->labelBornOn->setEnabled(true);
        ui->dateEditBirthDate->setDate(QDate::currentDate());
        birthDate=QDate::currentDate();
    }
    else if (arg1==Qt::Checked) {
        ui->dateEditBirthDate->setEnabled(false);
        ui->checkBoxAddBirthdayToCal->setEnabled(false);
        ui->labelBornOn->setEnabled(false);
        this->birthDate=QDate(); //null
        addToCalendar=0;
    }

}

void DialogContact::on_dateEditBirthDate_userDateChanged(const QDate &date)
{
    this->birthDate=date;
}

void DialogContact::on_checkBoxAddBirthdayToCal_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        addToCalendar=0;
    }
    else if (arg1==Qt::Checked) {
        addToCalendar=1;
    }
}

void DialogContact::on_checkBoxDelete_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        deleteRequested=false;
    }
    else if (arg1==Qt::Checked) {
        deleteRequested =true;
    }
}
