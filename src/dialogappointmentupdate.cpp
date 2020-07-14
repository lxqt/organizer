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

#include "dialogappointmentupdate.h"
#include "ui_dialogappointmentupdate.h"


DialogAppointmentUpdate::DialogAppointmentUpdate(QWidget *parent, Appointment *theAppointment) :
    QDialog(parent),
    ui(new Ui::DialogAppointmentUpdate)
{
    ui->setupUi(this);
    //Update an appointment

    ui->checkBoxDelete->show();

    //Set up Reminder ComboBox
    ui->comboBoxReminder->addItem(tr("5 Minutes Before Start"));
    ui->comboBoxReminder->addItem(tr("10 Minutes Before Start"));
    ui->comboBoxReminder->addItem(tr("30 Minutes Before Start"));
    ui->comboBoxReminder->addItem(tr("1 Hour Before Start"));
    ui->comboBoxReminder->addItem(tr("1 Day Before Start"));

    //populate appointment details in dialog
    this->titleValue=theAppointment->m_title;
    ui->lineEditTitle->setText(this->titleValue);
    this->locationValue=theAppointment->m_location;
    ui->lineEditLocation->setText(this->locationValue);
    this->notesValue=theAppointment->m_description;
    ui->textEditDescription->setText(this->notesValue);
    this->startTime=QTime::fromString(theAppointment->m_startTime);
    this->endTime =QTime::fromString(theAppointment->m_endTime);
    ui->timeEditStartTime->setTime(startTime);
    ui->timeEditEndTime->setTime(endTime);

    this->appointmentDate=QDate::fromString(theAppointment->m_date);

//    QString date =locale.toString(appointmentDate,QStringLiteral("dddd dd MMMM yyyy"));
//    ui->labelDateValue->setText(date);



    isAllDay=theAppointment->m_isFullDay;

    if (isAllDay==1)
    {
        ui->checkBoxAllDay->setCheckState(Qt::Checked);
    }

    valid =false;
    this->hasReminder=theAppointment->m_hasReminder;
    this->reminderMins=theAppointment->m_reminderMinutes;



    if (hasReminder==1)
    {
        ui->checkBoxReminder->setCheckState(Qt::Checked);
        ui->comboBoxReminder->setEnabled(true);

        if(reminderMins==5){
            ui->comboBoxReminder->setCurrentIndex(0);
        }
        else if(reminderMins==10){
            ui->comboBoxReminder->setCurrentIndex(1);
        }
        else if(reminderMins==30){
            ui->comboBoxReminder->setCurrentIndex(2);
        }
        else if(reminderMins==60){
            ui->comboBoxReminder->setCurrentIndex(3);
        }
        else if(reminderMins==1440){
            ui->comboBoxReminder->setCurrentIndex(4);
        }
    }
    else {
        ui->checkBoxReminder->setCheckState(Qt::Unchecked);
        ui->comboBoxReminder->setEnabled(false);
    }
}



DialogAppointmentUpdate::~DialogAppointmentUpdate()
{
    delete ui;
}

QString DialogAppointmentUpdate::getTitle()
{

    return this->titleValue;
}

QString DialogAppointmentUpdate::getLocation()
{
     return this->locationValue;
}

QString DialogAppointmentUpdate::getDescription()
{
    return this->notesValue;
}

QDate DialogAppointmentUpdate::getAppointmentDate()
{
    return this->appointmentDate;
}

QTime DialogAppointmentUpdate::getStartTime()
{
    return this->startTime;
}

QTime DialogAppointmentUpdate::getEndTime()
{
    return this->endTime;
}

int DialogAppointmentUpdate::getAllDay()
{
    return this->isAllDay;
}



bool DialogAppointmentUpdate::getDeleteRequested()
{
    return this->deleteRequested;

}

int DialogAppointmentUpdate::getReminderMins()
{
    return reminderMins;
}

int DialogAppointmentUpdate::getHasReminder()
{
    return hasReminder;
}


void DialogAppointmentUpdate::setLocaleDate(QLocale locale)
{
    this->locale=locale;
    QString date =locale.toString(appointmentDate,QStringLiteral("dddd dd MMMM yyyy"));
    ui->labelDateValue->setText(date);
}

void DialogAppointmentUpdate::accept()
{


    //return original values if not changed

    if (valid==false)
    {
        QMessageBox::information(this, QStringLiteral("No Updates Made"),QStringLiteral("Press Cancel to Quit"));
        return;

        //QDialog::reject();
    }
     else {
     QDialog::accept();

    }

}

void DialogAppointmentUpdate::reject()
{
    //Made no changes (return original values?)

    //qDebug("Cancel button pressed");
    QDialog::reject();

}



void DialogAppointmentUpdate::on_lineEditTitle_textChanged(const QString &arg1)
{

    this->titleValue =arg1;
    valid=true;
}

void DialogAppointmentUpdate::on_lineEditLocation_textChanged(const QString &arg1)
{

    this->locationValue=arg1;
    valid=true;
}

void DialogAppointmentUpdate::on_textEditDescription_textChanged()
{
    this->notesValue =ui->textEditDescription->toPlainText();
    valid =true;
}

void DialogAppointmentUpdate::on_checkBoxAllDay_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
        {
            isAllDay=0;
            ui->timeEditStartTime->setEnabled(true);
            ui->timeEditEndTime->setEnabled(true);
            ui->labelStartTime->setEnabled(true);
            ui->labelEndTime->setEnabled(true);


        }
        else if (arg1==Qt::Checked) {
            isAllDay=1;
            ui->timeEditStartTime->setEnabled(false);
            ui->timeEditEndTime->setEnabled(false);
            ui->labelStartTime->setEnabled(false);
            ui->labelEndTime->setEnabled(false);
        }

    valid=true;

}

void DialogAppointmentUpdate::on_timeEditStartTime_userTimeChanged(const QTime &time)
{
    this->startTime=time;
    valid=true;
}

void DialogAppointmentUpdate::on_timeEditEndTime_userTimeChanged(const QTime &time)
{
    this->endTime=time;
    valid =true;
}

void DialogAppointmentUpdate::on_checkBoxDelete_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        deleteRequested=false;
    }
    else if (arg1==Qt::Checked) {
        deleteRequested =true;
    }

    valid =true;

}


void DialogAppointmentUpdate::on_comboBoxReminder_currentIndexChanged(int index)
{
   valid=true;
    if(index==0){
       reminderMins=5;


    }
    else if(index==1){
       reminderMins=10;

    }
    else if(index==2){
        reminderMins=30;
       ;
    }
    else if(index==3){
        reminderMins=60; //1 hour

    }
    else if(index==4){
        reminderMins=1440; //minutes in a day i.e. 1 day = (24 hours/day) × (60 minutes/hour) = 1440

    }
}

void DialogAppointmentUpdate::on_checkBoxReminder_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {

        ui->comboBoxReminder->setEnabled(false);
        hasReminder=0;
        reminderMins=0;
        valid=true;
    }
    else if (arg1==Qt::Checked) {

        ui->comboBoxReminder->setEnabled(true);
        hasReminder=1;
        //reminderMins=5;
        valid=true;
    }
}
