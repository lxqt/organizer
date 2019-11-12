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
#include "dialogaddappointment.h"
#include "ui_dialogaddappointment.h"

DialogAddAppointment::DialogAddAppointment(QWidget *parent, QDate *theAppointmentDate) :
    QDialog(parent),
    ui(new Ui::DialogAddAppointment)
{
    ui->setupUi(this);
    setWindowTitle("New Appointment");
    ui->labelSelectedAppointmentDate->setText(theAppointmentDate->toString());
    ui->calendarWidgetDialog->setSelectedDate(*theAppointmentDate);
    ui->dateEditReminder->setDate(*theAppointmentDate);
    this->appointmentDate=*theAppointmentDate;
    ui->checkBoxReminder->setCheckState(Qt::Unchecked);
    ui->dateEditReminder->setDisabled(true);
    ui->timeEditReminder->setDisabled(true);
    reminderRequested=0;

   this->appointmentDate=*theAppointmentDate;
   this->startTime=ui->timeEditStartTime->time();
   this->endTime=ui->timeEditEndTime->time();
   this->reminderDate=ui->dateEditReminder->date();
   this->reminderTime=ui->timeEditReminder->time();

}

DialogAddAppointment::~DialogAddAppointment()
{
    delete ui;
}

QString DialogAddAppointment::getTitle()
{
    return ui->lineEditTitle->text();
}

QString DialogAddAppointment::getLocation()
{
    return ui->lineEditLocation->text();
}

QString DialogAddAppointment::getDescription()
{
    return ui->textEditDescription->toPlainText();
}

QDate DialogAddAppointment::getAppointmentDate()
{
    return this->appointmentDate;
}

QTime DialogAddAppointment::getStartTime()
{
    return this->startTime;
}

QTime DialogAddAppointment::getEndTime()
{
    return this->endTime;
}

QDate DialogAddAppointment::getReminderDate()
{
    return this->reminderDate;
}

QTime DialogAddAppointment::getReminderTime()
{
    return this->reminderTime;
}

int DialogAddAppointment::getReminderRequested()
{
    return reminderRequested;
}

void DialogAddAppointment::accept()
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

void DialogAddAppointment::on_checkBoxReminder_stateChanged(int arg1)
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


void DialogAddAppointment::on_timeEditStartTime_userTimeChanged(const QTime &time)
{
    this->startTime=time;
    ui->timeEditReminder->setTime(time);
}

void DialogAddAppointment::on_timeEditEndTime_userTimeChanged(const QTime &time)
{
    this->endTime=time;
}

void DialogAddAppointment::on_dateEditReminder_userDateChanged(const QDate &date)
{
    this->reminderDate=date;
}

void DialogAddAppointment::on_timeEditReminder_userTimeChanged(const QTime &time)
{
    this->reminderTime=time;
}

void DialogAddAppointment::on_calendarWidgetDialog_clicked(const QDate &date)
{
    this->appointmentDate=date;
    ui->labelSelectedAppointmentDate->setText(appointmentDate.toString());
    ui->dateEditReminder->setDate(date);
}
