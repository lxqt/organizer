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
    setWindowTitle(QStringLiteral("New Appointment"));

    ui->dateEditAppointmentDate->setDate(*theAppointmentDate);

    this->appointmentDate=*theAppointmentDate;
    ui->labelDateDisplay->setText(this->appointmentDate.toString());

    QTime timeNow =QTime::currentTime();
    this->startTime=timeNow;
    this->endTime=timeNow;
    ui->timeEditStartTime->setTime(timeNow);
    ui->timeEditEndTime->setTime(timeNow);

    //Translation refactoring
    ui->labelDate->setText(QStringLiteral("AppointmentDate: "));
    ui->labelTitle->setText(QStringLiteral("Title: "));
    ui->labelLocation->setText(QStringLiteral("Location: "));
    ui->labelDescription->setText(QStringLiteral("Notes: "));
    ui->checkBoxAllDay->setText(QStringLiteral("All Day"));
    ui->labelStarts->setText(QStringLiteral("Start Time: "));
    ui->labelEndTime->setText(QStringLiteral("End Time: "));
    ui->labelCategory->setText(QStringLiteral("Category "));

    ui->checkBoxReminder->setText(QStringLiteral("Reminder"));


    category=QStringLiteral("General");
    setupComboBoxes();

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

QString DialogAppointment::getCategory()
{
    return this->category;
}


bool DialogAppointment::getDeleteRequested()
{
    return this->deleteRequested;
}

void DialogAppointment::setupComboBoxes()
{
    //Set up Category Combobox

    ui->comboBoxCategory->addItem(QStringLiteral("General"));
    ui->comboBoxCategory->addItem(QStringLiteral("Meeting"));
    ui->comboBoxCategory->addItem(QStringLiteral("Work"));
    ui->comboBoxCategory->addItem(QStringLiteral("Family"));
    ui->comboBoxCategory->addItem(QStringLiteral("Leisure"));
    ui->comboBoxCategory->addItem(QStringLiteral("Fitness"));
    ui->comboBoxCategory->addItem(QStringLiteral("Vacation"));
    ui->comboBoxCategory->addItem(QStringLiteral("Medical"));

    //Set up Reminder ComboBox
    ui->comboBoxReminder->addItem(QStringLiteral("5 minutes before start"));
    ui->comboBoxReminder->addItem(QStringLiteral("10 minutes before start"));
    ui->comboBoxReminder->addItem(QStringLiteral("30 minutes before start"));
    ui->comboBoxReminder->addItem(QStringLiteral("1 hour before start"));
    ui->comboBoxReminder->addItem(QStringLiteral("1 day before start"));

}

int DialogAppointment::getReminderMins()
{
    return this->reminderMins;
}

int DialogAppointment::getHasReminder()
{
    return this->hasReminder;
}

void DialogAppointment::accept()
{

    qDebug("ok button pressed");


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

    qDebug("rejecting ... Cancel button pressed");

    QDialog::reject();
}

void DialogAppointment::on_dateEditAppointmentDate_userDateChanged(const QDate &date)
{
    this->appointmentDate=date;    
    ui->labelDateDisplay->setText(this->appointmentDate.toString());
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

void DialogAppointment::on_comboBoxCategory_activated(const QString &arg1)
{
     category=arg1;
     //qDebug()<<"Category = "<<category;

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
       //qDebug()<<"Reminder minutes = "<<reminderMins;
    }
    else if(index==1){
       reminderMins=10;
       //qDebug()<<"Reminder minutes = "<<reminderMins;
    }
    else if(index==2){
        reminderMins=30;
        qDebug()<<"Reminder minutes = "<<reminderMins;
    }
    else if(index==3){
        reminderMins=60;
        //qDebug()<<"Reminder minutes = "<<reminderMins;
    }
    else if(index==4){
        reminderMins=1440; //minutes in a day i.e. 1 day = (24 hours/day) × (60 minutes/hour) = 1440
        //qDebug()<<"Reminder minutes = "<<reminderMins;
    }
}

void DialogAppointment::on_checkBoxReminder_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        hasReminder=0;
        reminderMins=0;
       // qDebug()<<"Reminder miniutes = "<<reminderMins;
        ui->comboBoxReminder->setEnabled(false);


    }
    else if (arg1==Qt::Checked) {
        hasReminder=1;
        reminderMins=5;
        //qDebug()<<"Reminder minutes = "<<reminderMins;
        ui->comboBoxReminder->setEnabled(true);

    }
}
