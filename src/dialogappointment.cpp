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

#include "dialogappointment.h"
#include "ui_dialogappointment.h"


DialogAppointment::DialogAppointment(QWidget *parent, QDate *theAppointmentDate):
    QDialog(parent),
    ui(new Ui::DialogAppointment)
{
    ui->setupUi(this);
    //New appointment

    this->appointmentDate=*theAppointmentDate;
    //ui->labelDateValue->setText(this->appointmentDate.toString());

    QString date =locale.toString(appointmentDate,QStringLiteral("dddd dd MMMM yyyy"));
    ui->labelDateValue->setText(date);

    QTime timeNow =QTime::currentTime();
    this->startTime=timeNow;
    this->endTime=timeNow;
    ui->timeEditStartTime->setTime(timeNow);
    ui->timeEditEndTime->setTime(timeNow);
    ui->checkBoxReminder->setText(QStringLiteral("Reminder"));

    ui->checkBoxReminder->setCheckState(Qt::Unchecked);
    ui->comboBoxReminder->setEnabled(false);
}


DialogAppointment::~DialogAppointment()
{
    delete ui;
}


QString DialogAppointment::getTitle()
{
    //currentTitle=ui->lineEditTitle->text();
    return ui->lineEditTitle->text();
}

QString DialogAppointment::getLocation()
{
    return ui->lineEditLocation->text();
}

QString DialogAppointment::getDescription()
{
    return ui->textEditDescription->toPlainText();
}

QDate DialogAppointment::getAppointmentDate()
{
    return this->appointmentDate;
}

QTime DialogAppointment::getStartTime()
{
     return this->startTime;
}

QTime DialogAppointment::getEndTime()
{
    return this->endTime;
}

int DialogAppointment::getAllDay()
{
    return isAllDay;
}




bool DialogAppointment::getDeleteRequested()
{
    return this->deleteRequested;
}


int DialogAppointment::getReminderMins()
{
    return this->reminderMins;
}


void DialogAppointment::setLocaleDate(QLocale locale)
{
    this->locale=locale;
    QString date =locale.toString(appointmentDate,QStringLiteral("dddd dd MMMM yyyy"));
    ui->labelDateValue->setText(date);
}


int DialogAppointment::getHasReminder()
{
    return this->hasReminder;
}

void DialogAppointment::accept()
{

    //qDebug("ok button pressed");


    if (this->getTitle().isEmpty() || this->getLocation().isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("Empty Details"),
                                 QStringLiteral("Must enter a title and location"));
        return;
    }
    else {
        //ui->buttonBox->setEnabled(true);
        QDialog::accept();
    }
}

void DialogAppointment::reject()
{
    //keep all the old vararialbes

    //qDebug("rejecting ... Cancel button pressed");

    QDialog::reject();
}


void DialogAppointment::on_timeEditStartTime_userTimeChanged(const QTime &time)
{
    this->startTime=time;
    //ui->timeEditEndTime->setTime(time);
}

void DialogAppointment::on_timeEditEndTime_userTimeChanged(const QTime &time)
{
    this->endTime=time;
}



void DialogAppointment::on_checkBoxAllDay_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        isAllDay=0;
        ui->timeEditStartTime->setEnabled(true);
        ui->timeEditEndTime->setEnabled(true);
        ui->labelStarts->setEnabled(true);
        ui->labelEndTime->setEnabled(true);

    }
    else if (arg1==Qt::Checked) {
        isAllDay=1;
        ui->timeEditStartTime->setEnabled(false);
        ui->timeEditEndTime->setEnabled(false);
        ui->labelStarts->setEnabled(false);
        ui->labelEndTime->setEnabled(false);
    }
}


void DialogAppointment::on_comboBoxReminder_currentIndexChanged(int index)
{
    if(index==0){
       reminderMins=5;

    }
    else if(index==1){
       reminderMins=10;

    }
    else if(index==2){
        reminderMins=30;

    }
    else if(index==3){
        reminderMins=60;

    }
    else if(index==4){
        reminderMins=1440; //minutes in a day i.e. 1 day = (24 hours/day) × (60 minutes/hour) = 1440

    }
}

void DialogAppointment::on_checkBoxReminder_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        hasReminder=0;
        reminderMins=0;

        ui->comboBoxReminder->setEnabled(false);


    }
    else if (arg1==Qt::Checked) {
        hasReminder=1;
        reminderMins=5;

        ui->comboBoxReminder->setEnabled(true);

    }
}
