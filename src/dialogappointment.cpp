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

#include "dialogappointment.h"
#include "ui_dialogappointment.h"


DialogAppointment::DialogAppointment(QWidget *parent, QDate *theAppointmentDate):
    QDialog(parent),
    ui(new Ui::DialogAppointment)
{
    ui->setupUi(this);
    //New appointment
    setWindowTitle("New Appointment");
    ui->checkBoxDelete->hide();

    ui->dateEditAppointmentDate->setDate(*theAppointmentDate);
    ui->dateEditReminder->setDate(*theAppointmentDate);

    ui->checkBoxReminder->setCheckState(Qt::Unchecked);
    //ui->dateEditReminder->setDisabled(true);
    //ui->timeEditReminder->setDisabled(true);
    ui->dateEditReminder->hide();
    ui->timeEditReminder->hide();
    reminderRequested=0;



    QTime timeNow =QTime::currentTime();

    this->appointmentDate=*theAppointmentDate;
    this->reminderDate=*theAppointmentDate;

    ui->labelDateDisplay->setText(this->appointmentDate.toString());

    ui->dateEditReminder->setDate(*theAppointmentDate);

    this->startTime=timeNow;
    this->endTime=timeNow;
    ui->timeEditStartTime->setTime(timeNow);
    ui->timeEditEndTime->setTime(timeNow);
    this->reminderTime=timeNow;
    ui->timeEditReminder->setTime(timeNow);



}

DialogAppointment::DialogAppointment(QWidget *parent, Appointment *theAppointment) :
    QDialog(parent),
    ui(new Ui::DialogAppointment)
{
    ui->setupUi(this);
    //Update an appointment

    setWindowTitle("Update Appointment");
    ui->checkBoxDelete->show();

    ui->lineEditTitle->setText(theAppointment->m_title);
    ui->lineEditLocation->setText(theAppointment->m_location);
    ui->textEditDescription->setText(theAppointment->m_description);

    this->appointmentDate=QDate::fromString(theAppointment->m_appointmentDate);
    //QDate appointmentDate=QDate::fromString(theAppointment->m_appointmentDate);
    ui->dateEditAppointmentDate->setDate(this->appointmentDate);
    ui->labelDateDisplay->setText(this->appointmentDate.toString());

    this->startTime=QTime::fromString(theAppointment->m_appointmentStartTime);
    this->endTime =QTime::fromString(theAppointment->m_appointmentEndTime);

    ui->timeEditStartTime->setTime(startTime);
    ui->timeEditEndTime->setTime(endTime);

    //Testing

    ui->dateEditReminder->setDate(this->appointmentDate);
    ui->timeEditReminder->setTime(this->startTime);
    ui->dateEditReminder->hide();
    ui->timeEditReminder->hide();


//    int r = theAppointment->m_reminderRequest;
//    if (r==1)
//    {
//        ui->checkBoxReminder->setCheckState(Qt::CheckState::Checked);
//    }

//    this->reminderDate=QDate::fromString(theAppointment->m_reminderDate);
//    this->reminderTime=QTime::fromString(theAppointment->m_reminderTime);
//    ui->dateEditReminder->setDate(reminderDate);
//    ui->timeEditReminder->setTime(reminderTime);

}

DialogAppointment::~DialogAppointment()
{
    delete ui;
}

QString DialogAppointment::getTitle()
{
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

QDate DialogAppointment::getReminderDate()
{
     return this->reminderDate;
}

QTime DialogAppointment::getReminderTime()
{
    return this->reminderTime;
}

int DialogAppointment::getReminderRequested()
{
    return this->reminderRequested;
}

bool DialogAppointment::getDeleteRequested()
{
    return this->deleteRequested;
}

void DialogAppointment::accept()
{
    if (this->getTitle().isEmpty() || this->getLocation().isEmpty())
    {
        QMessageBox::information(this, tr("Empty Details"),
                                 tr("Must enter a title and location"));
        return;
    }
    else {
        //        QMessageBox::information(this, tr("Success"),
        //                  tr("Data entered"));
        qDebug()<<"Success title and location fields completed by user";
        QDialog::accept();
    }
}

void DialogAppointment::on_dateEditAppointmentDate_userDateChanged(const QDate &date)
{
    this->appointmentDate=date;
    ui->dateEditReminder->setDate(date);
    ui->labelDateDisplay->setText(this->appointmentDate.toString());
}

void DialogAppointment::on_timeEditStartTime_userTimeChanged(const QTime &time)
{
    this->startTime=time;
    ui->timeEditEndTime->setTime(time);
    ui->timeEditReminder->setTime(time);

}

void DialogAppointment::on_timeEditEndTime_userTimeChanged(const QTime &time)
{
    this->endTime=time;
}

void DialogAppointment::on_dateEditReminder_userDateChanged(const QDate &date)
{
    this->reminderDate=date;
}

void DialogAppointment::on_checkBoxReminder_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        //enable reminder date time edit boxes
        //            ui->dateEditReminder->setEnabled(false);
        //            ui->timeEditReminder->setEnabled(false);
        ui->dateEditReminder->hide();
        ui->timeEditReminder->hide();

        reminderRequested=0;
    }
    else if (arg1==Qt::Checked) {
        //disable reminder date and time edit boxes
        //ui->dateEditReminder->setEnabled(true);
        //ui->timeEditReminder->setEnabled(true);
        ui->dateEditReminder->show();
        ui->timeEditReminder->show();
        reminderRequested=1;
    }
    qDebug()<<"ReminderRequested = "<<reminderRequested;
}

void DialogAppointment::on_timeEditReminder_userTimeChanged(const QTime &time)
{
    this->reminderTime=time;
}


void DialogAppointment::on_checkBoxDelete_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
        deleteRequested=false;
    }
    else if (arg1==Qt::Checked) {
        deleteRequested =true;
    }
}
