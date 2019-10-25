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

#include "dialogaddevent.h"
#include "ui_dialogaddevent.h"

DialogAddEvent::DialogAddEvent(QWidget *parent,QDate *eventDate) :
    QDialog(parent),
    ui(new Ui::DialogAddEvent)
{
    ui->setupUi(this);
    setWindowTitle("Add Event");
    ui->labelSelectedEventDate->setText(eventDate->toString());
    ui->dateEdit->setDate(*eventDate);
    this->eventDate=*eventDate;
    setupComboBoxes();

}

DialogAddEvent::~DialogAddEvent()
{
    delete ui;
}

QString DialogAddEvent::getTitle()
{
    return ui->lineEditTitle->text();
}

QString DialogAddEvent::getLocation()
{
    return ui->lineEditLocation->text();
}

QDate DialogAddEvent::getEventDate()
{
    return eventDate;
}

int DialogAddEvent::getStartTime()
{
    return startTime;
}

int DialogAddEvent::getEndTime()
{

    return endTime;
}

QDate DialogAddEvent::getReminderDate()
{
    return reminderDate;
}

int DialogAddEvent::getReminderTime()
{
    return reminderTime;
}


void DialogAddEvent::setupComboBoxes()
{
    //Setup start and end time ComboBoxes 24 hour clock
       for (int i=1; i<25; i++)
       {
           QString timeStr =QString::number(i)+":00";
           ui->comboBoxStartTime->addItem(timeStr);
           ui->comboBoxEndTime->addItem(timeStr);
           ui->comboBoxReminderTime->addItem(timeStr);
       }
       ui->comboBoxStartTime->setCurrentIndex(8);
       ui->comboBoxEndTime->setCurrentIndex(9);
       ui->comboBoxReminderTime->setCurrentIndex(9);
       startTime=ui->comboBoxStartTime->currentIndex()+1;
       endTime=ui->comboBoxEndTime->currentIndex()+1;
       reminderTime=ui->comboBoxReminderTime->currentIndex()+1;

       //Set up reminder days ComboBox
       for (int i=0; i<8; i++)
       {
           QString dayReminderStr =QString::number(i)+" days before";
           ui->comboBoxReminderDays->addItem(dayReminderStr);
       }
       ui->comboBoxReminderDays->setCurrentIndex(0);
       reminderDays = ui->comboBoxReminderDays->currentIndex()+1;

       reminderDate =eventDate; //initialise

}


void DialogAddEvent::on_dateEdit_userDateChanged(const QDate &date)
{
   eventDate=date;
   ui->labelSelectedEventDate->setText(eventDate.toString());
}

void DialogAddEvent::on_comboBoxStartTime_activated(int index)
{
    startTime=index+1;
}

void DialogAddEvent::on_comboBoxEndTime_activated(int index)
{
     endTime=index+1;
}

void DialogAddEvent::on_comboBoxReminderDays_activated(int index)
{
    reminderDays=index;
    reminderDate=eventDate.addDays(-reminderDays);
    //if reminder date before current date message error
    QDate currentDate = QDate::currentDate();

    if (reminderDate.operator<(currentDate)){
        QMessageBox::information( this, "Date Error",
                                  ("You cannot set a reminder date before current date"
                                   "\nLXQt Team") );
        reminderDate=currentDate;
        ui->comboBoxReminderDays->setCurrentIndex(0);
    }
}

void DialogAddEvent::on_comboBoxReminderTime_activated(int index)
{
     reminderTime=index+1;
}
