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
    setWindowTitle(tr("New Appointment"));

    ui->dateEditAppointmentDate->setDate(*theAppointmentDate);

    this->appointmentDate=*theAppointmentDate;
    ui->labelDateDisplay->setText(this->appointmentDate.toString());

    QTime timeNow =QTime::currentTime();
    this->startTime=timeNow;
    this->endTime=timeNow;
    ui->timeEditStartTime->setTime(timeNow);
    ui->timeEditEndTime->setTime(timeNow);

    //Translation refactoring
    ui->labelDate->setText((tr("AppointmentDate: ")));
    ui->labelTitle->setText(tr("Title: "));
    ui->labelLocation->setText(tr("Location: "));
    ui->labelDescription->setText(tr("Notes: "));
    ui->checkBoxAllDay->setText(tr("All Day"));
    ui->labelStarts->setText(tr("Start Time: "));
    ui->labelEndTime->setText(tr("End Time: "));
    ui->labelCategory->setText(tr("Category "));
    category=QStringLiteral("General");
    setupComboBoxes();
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

    ui->comboBoxCategory->addItem(tr("General"));    
    ui->comboBoxCategory->addItem(tr("Meeting"));
    ui->comboBoxCategory->addItem(tr("Work"));    
    ui->comboBoxCategory->addItem(tr("Family"));
    ui->comboBoxCategory->addItem(tr("Leisure"));
    ui->comboBoxCategory->addItem(tr("Fitness"));
    ui->comboBoxCategory->addItem(tr("Vacation"));
    ui->comboBoxCategory->addItem(tr("Medical"));
}

void DialogAppointment::accept()
{

    qDebug("ok button pressed");


    if (this->getTitle().isEmpty() || this->getLocation().isEmpty())
    {
        QMessageBox::information(this, tr("Empty Details"),
                                 tr("Must enter a title and location"));
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

