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
    setWindowTitle(QStringLiteral("Update Appointment"));
    ui->checkBoxDelete->show();

    this->title=theAppointment->m_title;
    ui->lineEditTitle->setText(this->title);
    this->location=theAppointment->m_location;
    ui->lineEditLocation->setText(this->location);

    this->description=theAppointment->m_description;
    ui->textEditDescription->setText(this->description);

    this->appointmentDate=QDate::fromString(theAppointment->m_date);

    //QDate appointmentDate=QDate::fromString(theAppointment->m_appointmentDate);
    ui->dateEditAppointmentDate->setDate(this->appointmentDate);
    ui->labelDateDisplay->setText(this->appointmentDate.toString());


    this->startTime=QTime::fromString(theAppointment->m_startTime);
    this->endTime =QTime::fromString(theAppointment->m_endTime);

    ui->timeEditStartTime->setTime(startTime);
    ui->timeEditEndTime->setTime(endTime);

    setupComboBoxes();
    category=theAppointment->m_category;

    int index = ui->comboBoxCategory->findText(category);

    if ( index != -1 ) { // -1 for not found must be >0
       ui->comboBoxCategory->setCurrentIndex(index);
    }

    isAllDay=theAppointment->m_isFullDay;

    if (isAllDay==1)
    {
        ui->checkBoxAllDay->setCheckState(Qt::Checked);
    }

    //Translation refactoring
    ui->labelDate->setText(QStringLiteral("AppointmentDate: "));
    ui->labelTitle->setText(QStringLiteral("Title: "));
    ui->labelLocation->setText(QStringLiteral("Location: "));
    ui->labelDescription->setText(QStringLiteral("Notes: "));
    ui->checkBoxAllDay->setText(QStringLiteral("All Day"));
    ui->labelStartTime->setText(QStringLiteral("Start Time: "));
    ui->labelEndTime->setText(QStringLiteral("End Time: "));
    ui->labelCategory->setText(QStringLiteral("Category "));
    ui->checkBoxReminder->setText(QStringLiteral("Reminder"));
    valid =false;
    //connect( ui->buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
    //connect( ui->buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

    //ui->checkBoxReminder->setCheckState(Qt::Unchecked);
    //ui->comboBoxReminder->setEnabled(false);

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

    return this->title;
}

QString DialogAppointmentUpdate::getLocation()
{
     return this->location;
}

QString DialogAppointmentUpdate::getDescription()
{
    return this->description;
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

QString DialogAppointmentUpdate::getCategory()
{
    return this->category;
}

bool DialogAppointmentUpdate::getDeleteRequested()
{
    return this->deleteRequested;

}

void DialogAppointmentUpdate::setupComboBoxes()
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

    //ui->comboBoxReminder->addItem(tr("1 hour before start"));
    //ui->comboBoxReminder->addItem(tr("1 day before start"));
    //ui->comboBoxReminder->addItem(tr("2 days before start"));
}

int DialogAppointmentUpdate::getReminderMins()
{
    return reminderMins;
}

int DialogAppointmentUpdate::getHasReminder()
{
    return hasReminder;
}

void DialogAppointmentUpdate::accept()
{
    qDebug("ok button pressed");
    qDebug()<<"valid = "<<valid;

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

//    this->title=ui->lineEditTitle->text();
//    this->location=ui->lineEditLocation->text();
//    this->description=ui->textEditDescription->toPlainText();
//    this->isAllDay=ui->checkBoxAllDay->checkState();
//    this->startTime=ui->timeEditStartTime->time();
//    this->endTime=ui->timeEditEndTime->time();
//    this->category=ui->comboBoxCategory->itemText(categoryIndex);
//    this->deleteRequested=ui->checkBoxDelete->checkState();



}

void DialogAppointmentUpdate::reject()
{
    //Made no changes (return original values?)

    //qDebug("Cancel button pressed");
    QDialog::reject();

}

void DialogAppointmentUpdate::on_comboBoxCategory_activated(int index)
{
    this->categoryIndex=index;
}

void DialogAppointmentUpdate::on_lineEditTitle_textChanged(const QString &arg1)
{
    //qDebug()<<"Title = "<<arg1;
    this->title =arg1;
    valid=true;
}

void DialogAppointmentUpdate::on_lineEditLocation_textChanged(const QString &arg1)
{
    //qDebug()<<"Location = "<<arg1;
    this->location=arg1;
    valid=true;
}

void DialogAppointmentUpdate::on_textEditDescription_textChanged()
{
    this->description =ui->textEditDescription->toPlainText();
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

void DialogAppointmentUpdate::on_comboBoxCategory_currentIndexChanged(const QString &arg1)
{
    category=arg1;
    valid=true;
}

void DialogAppointmentUpdate::on_comboBoxReminder_currentIndexChanged(int index)
{
   valid=true;
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
        reminderMins=60; //1 hour
        //qDebug()<<"Reminder minutes = "<<reminderMins;
    }
    else if(index==4){
        reminderMins=1440; //minutes in a day i.e. 1 day = (24 hours/day) × (60 minutes/hour) = 1440
        //qDebug()<<"Reminder minutes = "<<reminderMins;
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
        reminderMins=5;
        valid=true;

    }
}
