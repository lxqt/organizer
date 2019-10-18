#include "dialogsetappointment.h"
#include "ui_dialogsetappointment.h"

DialogSetAppointment::DialogSetAppointment(QWidget *parent,  QDate *eventDate) :
    QDialog(parent),
    ui(new Ui::DialogSetAppointment)
{
    ui->setupUi(this);
    setWindowTitle("Add Appointment");
    ui->labelSelectedAppointmentDate->setText(eventDate->toString());
    this->eventDate=*eventDate;
    setupComboBoxes();

//    ui->comboBoxStartTime->setCurrentIndex(8);
//    ui->comboBoxEndTime->setCurrentIndex(9);
//    ui->comboBoxReminder->setCurrentIndex(0);

}

DialogSetAppointment::~DialogSetAppointment()
{
    delete ui;
}

QString DialogSetAppointment::getTitle()
{
    return ui->lineEditTitle->text();
}

QString DialogSetAppointment::getLocation()
{
    return ui->lineEditLocation->text();
}

int DialogSetAppointment::getStartTime()
{
    return startTime;
}

int DialogSetAppointment::getEndTime()
{
    return endTime;
}

QDate DialogSetAppointment::getReminderDate()
{
    return reminderDate;
}

int DialogSetAppointment::getReminderTime()
{
    return reminderTime;
}

void DialogSetAppointment::setupComboBoxes()
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
        ui->comboBoxReminder->addItem(dayReminderStr);
    }
    ui->comboBoxReminder->setCurrentIndex(0);
    reminderDays = ui->comboBoxReminder->currentIndex()+1;

    reminderDate =eventDate; //initialise

    //reminderDate=eventDate.addDays(-reminderDays);
    //qDebug()<<"Reminder date = "<<reminderDate;

}

void DialogSetAppointment::on_comboBoxStartTime_activated(int index)
{
    startTime=index+1;
    //ui->comboBoxEndTime->setCurrentIndex(index+2);
}

void DialogSetAppointment::on_comboBoxEndTime_activated(int index)
{
    endTime=index+1;
}

void DialogSetAppointment::on_comboBoxReminder_activated(int index)
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
        ui->comboBoxReminder->setCurrentIndex(0);
    }
}

void DialogSetAppointment::on_comboBoxReminderTime_activated(int index)
{
    reminderTime=index+1;
}
