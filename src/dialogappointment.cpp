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

    reminderDays=0;

    QTime timeNow =QTime::currentTime();

    this->appointmentDate=*theAppointmentDate;
    this->reminderDate=*theAppointmentDate;

    ui->labelDateDisplay->setText(this->appointmentDate.toString());

    this->startTime=timeNow;
    this->endTime=timeNow;
    ui->timeEditStartTime->setTime(timeNow);
    ui->timeEditEndTime->setTime(timeNow);
    this->reminderTime=timeNow;

    category="Event";
    setupComboBoxes();
    ui->comboBoxReminder->setEnabled(false);

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

    this->appointmentDate=QDate::fromString(theAppointment->m_date);
    //QDate appointmentDate=QDate::fromString(theAppointment->m_appointmentDate);
    ui->dateEditAppointmentDate->setDate(this->appointmentDate);
    ui->labelDateDisplay->setText(this->appointmentDate.toString());

    this->startTime=QTime::fromString(theAppointment->m_startTime);
    this->endTime =QTime::fromString(theAppointment->m_endTime);

    ui->timeEditStartTime->setTime(startTime);
    ui->timeEditEndTime->setTime(endTime);

    category=theAppointment->m_category;
    isAllDay=theAppointment->m_isFullDay;

    if (isAllDay==1)
    {
        ui->checkBoxAllDay->setCheckState(Qt::Checked);
    }

    reminderRequested=theAppointment->m_hasReminder;



    if(reminderRequested==1)
    {
        ui->checkBoxReminder->setCheckState(Qt::Checked);
        ui->comboBoxReminder->setEnabled(true);
        reminderDays=1; //reminder days to be stored in appointment?
    }
    else {
        ui->checkBoxReminder->setCheckState(Qt::Unchecked);
        ui->comboBoxReminder->setEnabled(false);
    }

    setupComboBoxes();

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

int DialogAppointment::getReminderDays()
{
    return reminderDays;
}


int DialogAppointment::getAllDay()
{
    return isAllDay;
}

QString DialogAppointment::getCategory()
{
    return this->category;
}

int DialogAppointment::getReminderRequested()
{
    return reminderRequested;
}


bool DialogAppointment::getDeleteRequested()
{
    return this->deleteRequested;
}

void DialogAppointment::setupComboBoxes()
{
    //Set up Category Combobox

    ui->comboBoxCategory->addItem("Event");
    ui->comboBoxCategory->addItem("Family");
    ui->comboBoxCategory->addItem("Leisure");
    ui->comboBoxCategory->addItem("Meeting");
    ui->comboBoxCategory->addItem("Business");
    ui->comboBoxCategory->addItem("Vacation");
    ui->comboBoxCategory->addItem("Medical");


    //Set up reminder days ComboBox
    for (int i=1; i<8; i++)
    {
        QString dayReminderStr =QString::number(i)+" days before";
        ui->comboBoxReminder->addItem(dayReminderStr);
    }
    ui->comboBoxReminder->setCurrentIndex(0);
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


void DialogAppointment::on_comboBoxCategory_activated(const QString &arg1)
{
     category=arg1;
     qDebug()<<"Category = "<<category;

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

        //ui->timeEditStartTime->show();
        //ui->timeEditEndTime->show();
        //ui->labelStarts->show();
        //ui->labelEndTime->show();


    }
    else if (arg1==Qt::Checked) {
        isAllDay=1;
        ui->timeEditStartTime->setEnabled(false);
        ui->timeEditEndTime->setEnabled(false);
        ui->labelStarts->setEnabled(false);
        ui->labelEndTime->setEnabled(false);

        //ui->timeEditStartTime->hide();
        //ui->labelStarts->hide();
        //ui->labelEndTime->hide();
    }

}

void DialogAppointment::on_comboBoxReminder_activated(int index)
{
     reminderDays=index+1;
     qDebug()<<"reminderDays = "<<reminderDays;
}

void DialogAppointment::on_checkBoxReminder_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {

        reminderRequested=0;
        ui->comboBoxReminder->setEnabled(false);
    }
    else if(arg1==Qt::Checked)
    {
        reminderRequested=1;
        reminderDays=1; //default
        ui->comboBoxReminder->setEnabled(true);
    }

}
