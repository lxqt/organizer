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

#include "dialogrepeatappointment.h"
#include "ui_dialogrepeatappointment.h"

DialogRepeatAppointment::DialogRepeatAppointment(QWidget *parent, QDate *theAppointmentDate):
    QDialog(parent),
    ui(new Ui::DialogRepeatAppointment)
{
    ui->setupUi(this);

    setWindowTitle(tr("Generate Repeat Appointments"));
    this->appointmentDate=*theAppointmentDate;

    //Translation refactoring
    ui->labelInfo->setText((tr("Appointment Start Date: ")));
    ui->labelTitle->setText(tr("Title: "));
    ui->labelLocation->setText(tr("Location: "));
    ui->labelNotes->setText(tr("Notes: "));
    ui->checkBox->setText(tr("All Day "));
    ui->labelStartTime->setText(tr("Start Time: "));
    ui->labelEndTime->setText(tr("End Time: "));
    ui->labelCategory->setText(tr("Category"));
    ui->labelRepeatEvery->setText(tr("Repeat Every: "));
    ui->labelDays->setText(tr("Days "));
    ui->labelOccurrences->setText(tr("Occurrences"));

    ui->labelDate->setText(appointmentDate.toString());
    ui->dateEdit->setDate(*theAppointmentDate);

    QTime timeNow =QTime::currentTime();
    this->startTime=timeNow;
    this->endTime=timeNow;
    ui->timeEditStarts->setTime(timeNow);
    ui->timeEditEnds->setTime(timeNow);

    category="Event";
    setupComboBoxes();

    ui->spinBoxDays->setRange(1,28); //upto 4 weeks apart
    ui->spinBoxDays->setValue(1);
    ui->spinBoxRepeatNumber->setRange(0,12); //over 3 months
    ui->spinBoxRepeatNumber->setValue(1);

}

DialogRepeatAppointment::~DialogRepeatAppointment()
{
    delete ui;
}

int DialogRepeatAppointment::getRepeatDayInterval()
{
    return repeatDayInterval;
}

int DialogRepeatAppointment::getRepeatNumber()
{
    return repeatNumber;
}

QString DialogRepeatAppointment::getTitle()
{
   return  ui->lineEditTitle->text();
}

QString DialogRepeatAppointment::getLocation()
{
    return ui->lineEditLocation->text();
}

QString DialogRepeatAppointment::getDescription()
{

    return ui->textEditDescription->toPlainText();
}

QDate DialogRepeatAppointment::getAppointmentDate()
{
    return this->appointmentDate;
}

QTime DialogRepeatAppointment::getStartTime()
{
    return this->startTime;
}

QTime DialogRepeatAppointment::getEndTime()
{
    return this->endTime;
}

int DialogRepeatAppointment::getAllDay()
{
    return isAllDay;
}

QString DialogRepeatAppointment::getCategory()
{
    return this->category;
}

void DialogRepeatAppointment::setupComboBoxes()
{
    //Set up Category Combobox

    ui->comboBoxCategory->addItem(tr("General"));
    ui->comboBoxCategory->addItem(tr("Family"));
    ui->comboBoxCategory->addItem(tr("Leisure"));
    ui->comboBoxCategory->addItem(tr("Meeting"));
    ui->comboBoxCategory->addItem(tr("Work"));
    ui->comboBoxCategory->addItem(tr("Vacation"));
    ui->comboBoxCategory->addItem(tr("Medical"));
}

void DialogRepeatAppointment::accept()
{
    if (this->getTitle().isEmpty() || this->getLocation().isEmpty())
    {
        QMessageBox::information(this, tr("Empty Details"),
                                 tr("Must enter a title and location"));
        return;
    }
    else {
        QDialog::accept();
    }
}


void DialogRepeatAppointment::on_comboBoxCategory_activated(const QString &arg1)
{
    category=arg1;
}

void DialogRepeatAppointment::on_timeEditStarts_userTimeChanged(const QTime &time)
{
    this->startTime=time;
}

void DialogRepeatAppointment::on_timeEditEnds_userTimeChanged(const QTime &time)
{
    this->endTime=time;
}

void DialogRepeatAppointment::on_checkBox_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
        {
            isAllDay=0;
            ui->timeEditStarts->setEnabled(true);
            ui->timeEditEnds->setEnabled(true);
            ui->labelStartTime->setEnabled(true);
            ui->labelEndTime->setEnabled(true);

        }
        else if (arg1==Qt::Checked) {
            isAllDay=1;
            ui->timeEditStarts->setEnabled(false);
            ui->timeEditEnds->setEnabled(false);
            ui->labelStartTime->setEnabled(false);
            ui->labelEndTime->setEnabled(false);
        }
}

void DialogRepeatAppointment::on_spinBoxDays_valueChanged(int arg1)
{
    repeatDayInterval=arg1;
}

void DialogRepeatAppointment::on_spinBoxRepeatNumber_valueChanged(int arg1)
{
    repeatNumber=arg1;
}
