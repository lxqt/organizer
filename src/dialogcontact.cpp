#include "dialogcontact.h"
#include "ui_dialogcontact.h"

DialogContact::DialogContact(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogContact)
{
    ui->setupUi(this);
    //New Contact
    setWindowTitle(tr("New Contact"));
    ui->checkBoxDelete->hide();
    this->birthDate=QDate(); //null

    ui->labelFirstName->setText(tr("First Name: "));
    ui->labelMidNames->setText(tr("Middle Names: "));
    ui->labelLastName->setText(tr("Last Name: "));
    ui->labelEmail->setText(tr("Email:"));
    ui->labelBornOn->setText(tr("Born On: "));
    ui->labelStreet->setText(tr("Street: "));
    ui->labelDistrict->setText(tr("District: "));
    ui->labelCity->setText(tr("City: "));
    ui->labelCounty->setText(tr("County: "));
    ui->labelPostcode->setText(tr("Postcode: "));
    ui->labelCountry->setText(tr("Country: "));
    ui->labelPhone->setText(tr("Phone: "));
    ui->checkBoxDelete->setText(tr("Delete Contact"));

    ui->checkBoxBirthdayUnknown->setText(tr("Birthday Unknown"));
    ui->checkBoxAddBirthdayToCal->setText(tr("Add To Calendar"));
    ui->checkBoxBirthdayUnknown->setCheckState(Qt::Unchecked);
    ui->dateEditBirthDate->setDate(QDate::currentDate());

}

DialogContact::DialogContact(Contact *theContact, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogContact)
{
    ui->setupUi(this);
    //Update Contact
    setWindowTitle(tr("Update Contact"));
    ui->checkBoxDelete->show();

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


    ui->labelFirstName->setText(tr("First Name: "));
    ui->labelMidNames->setText(tr("Middle Names: "));
    ui->labelLastName->setText(tr("Last Name: "));
    ui->labelEmail->setText(tr("Email:"));
    ui->labelBornOn->setText(tr("Born On: "));
    ui->labelStreet->setText(tr("Street: "));
    ui->labelDistrict->setText(tr("District: "));
    ui->labelCity->setText(tr("City: "));
    ui->labelCounty->setText(tr("County: "));
    ui->labelPostcode->setText(tr("Postcode: "));
    ui->labelCountry->setText(tr("Country: "));
    ui->labelPhone->setText(tr("Phone: "));
    ui->checkBoxDelete->setText(tr("Delete Contact"));
    ui->checkBoxAddBirthdayToCal->setText(tr("Add Birthday To Calendar"));
    ui->checkBoxBirthdayUnknown->setText(tr("Birthday Unknown"));

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

void DialogContact::accept()
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
