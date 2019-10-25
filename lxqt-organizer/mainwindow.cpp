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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qcalendarwidget.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    eventModel(),
    ui(new Ui::MainWindow)
     //appointmentModel()
{
    ui->setupUi(this);
    setWindowTitle("LXQt Organizer");
    dbm.openDatabase();
    dbm.createDatabaseTable();
   // id=0;
    initialiseDates();
    setCalendarOptions();
    ui->calendarWidget->update();
    showDbEventsAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialiseDates()
{
    //initialise event date
    selectedDate = ui->calendarWidget->selectedDate();
    day =selectedDate.day();
    month=selectedDate.month();
    year=selectedDate.year();
    ui->labelSelectedDate->setText(selectedDate.toString());

    //initialise reminder date
    reminderDays=0;
    reminderDate=selectedDate.addDays(-reminderDays);
    remDay=reminderDate.day();
    remMonth=reminderDate.month();
    remYear=reminderDate.year();
}

void MainWindow::showDbEventsAll()
{
    eventModel.clearAllEvents();//clear model
    //get appointments stored in database as a QList
    QList<Event> list=dbm.getAllAppointments();
    foreach(Event a, list){
        Event *atmp = new Event(a.id,
                                         a.title,
                                         a.location,
                                         a.day,
                                         a.month,
                                         a.year,
                                         a.startTime,
                                         a.endTime,
                                         a.reminderDay,
                                         a.reminderMonth,
                                         a.reminderYear,
                                         a.reminderTime);
       qDebug()<<"Database id = "<<a.id;
       eventModel.addEvent(atmp);
       QDate tmp=QDate(a.year,a.month,a.day);
       ui->calendarWidget->setDateTextFormat(tmp,eventDayFormat);
    }
    ui->listView->setModel(&eventModel); //display
}


void MainWindow::ShowDbEventsForDate(QDate &dairyDate)
{
    //resetCalendarColours();
    eventModel.clearAllEvents();//clear model
    //get appointments stored in database as a QList
    eventModel.clearAllEvents();
    QDate checkDate=QDate(dairyDate);
    QList<Event> list=dbm.getAppointmentsOnDate(&checkDate);
    foreach(Event a, list){
        Event *atmp = new Event(a.id,
                                         a.title,
                                         a.location,
                                         a.day,
                                         a.month,
                                         a.year,
                                         a.startTime,
                                         a.endTime,
                                         a.reminderDay,
                                         a.reminderMonth,
                                         a.reminderYear,
                                         a.reminderTime);
       qDebug()<<"Database id = "<<a.id;
       eventModel.addEvent(atmp);
       QDate tmp=QDate(a.year,a.month,a.day);
       ui->calendarWidget->setDateTextFormat(tmp,eventDayFormat);
    }
    ui->listView->setModel(&eventModel);
}

QList<Event> MainWindow::getAllAppointments()
{
    QList<Event> list=dbm.getAllAppointments();
    return list;
}

void MainWindow::setDefaultOptions()
{
    ui->actionNotifications->setChecked(true);
    this->notificationsFlag=true;
}

void MainWindow::setCalendarOptions()
{
   ui->calendarWidget->setGridVisible(true);
   ui->calendarWidget->setVerticalHeaderFormat(ui->calendarWidget->NoVerticalHeader);
   eventDayFormat.setForeground(Qt::magenta);
   eventDayFormat.setBackground(Qt::gray);
   weekDayFormat.setForeground(Qt::black);
   weekDayFormat.setBackground(Qt::white);
   weekendFormat.setForeground(Qt::red);
   weekendFormat.setBackground(Qt::white);

}

void MainWindow::resetCalendarColours()
{
    ui->calendarWidget->setWeekdayTextFormat(Qt::Monday, weekDayFormat);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Tuesday, weekDayFormat);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Wednesday, weekDayFormat);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Thursday, weekDayFormat);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Friday, weekDayFormat);

    ui->calendarWidget->setWeekdayTextFormat(Qt::Saturday, weekendFormat);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Sunday, weekendFormat);
    ui->calendarWidget->repaint();

}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout *aboutDialog = new DialogAbout(this);
    aboutDialog->setModal(false);
    aboutDialog->exec();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    ui->labelSelectedDate->setText(date.toString());
    //Show appointments for selected date
    selectedDate=date;
    QDate checkDate=QDate(date);
    ShowDbEventsForDate(checkDate);
}

void MainWindow::on_actionDelete_All_triggered()
{
    eventModel.clearAllEvents();
    ui->listView->setModel(&eventModel);
    dbm.removeAllAppointments();
}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    selectedRowIdx=index.row();
    Event *a =eventModel.getEvent(selectedRowIdx);    
    selectedDbId=a->id;
}

void MainWindow::on_actionNotifications_triggered()
{
    if(ui->actionNotifications->isChecked())
    {
        this->notificationsFlag =true;
    }
    else {
        this->notificationsFlag=false;
    }
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    selectedRowIdx=index.row();
    Event *a =eventModel.getEvent(selectedRowIdx);
    QDate tmp=QDate(a->year,a->month,a->day);    
    selectedDbId=a->id;
    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete Event");
    msgBox.setText("Are you sure you want to delete event?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
        //remove from database
        dbm.removeAppointmentById(selectedDbId);
        //remove from model
        eventModel.removeEvent(selectedRowIdx);
        ui->listView->setModel(&eventModel);
        //reset calendar colours

        if(tmp.dayOfWeek()==Qt::Saturday || tmp.dayOfWeek()==Qt::Sunday)
        {
            ui->calendarWidget->setDateTextFormat(tmp,weekendFormat);
        }
        else {
           ui->calendarWidget->setDateTextFormat(tmp,weekDayFormat);
        }

    }else {
      //qDebug()<<"abort delete event";
    }


}

void MainWindow::on_actionClear_List_triggered()
{
    eventModel.clearAllEvents();
    ui->listView->setModel(&eventModel);
}


void MainWindow::on_actionCalendar_Grid_triggered()
{

    if (ui->actionCalendar_Grid->isChecked())
    {
        ui->calendarWidget->setGridVisible(true);
    }
    else {
        ui->calendarWidget->setGridVisible(false);
    }
}

void MainWindow::on_actionCalendar_Weeks_triggered()
{
    if(ui->actionCalendar_Weeks->isChecked())
    {
        ui->calendarWidget->setVerticalHeaderFormat(ui->calendarWidget->ISOWeekNumbers);
    }
    else {
        ui->calendarWidget->setVerticalHeaderFormat(ui->calendarWidget->NoVerticalHeader);
    }
}

void MainWindow::on_actionAdd_triggered()
{
      addEvent();
}

void MainWindow::addEvent()
{
   // DialogSetAppointment *appointmentDialog = new DialogSetAppointment(this, &selectedDate);
   // appointmentDialog->setModal(true);
    DialogAddEvent *addEventDialog = new  DialogAddEvent(this,&selectedDate);
    addEventDialog->setModal(true);

    if (addEventDialog->exec() == QDialog::Accepted ) {

        title =addEventDialog->getTitle();
        location =addEventDialog->getLocation();

        selectedDate=addEventDialog->getEventDate();
        day=selectedDate.day();
        month =selectedDate.month();
        year =selectedDate.year();
        startTime=addEventDialog->getStartTime();
        endTime=addEventDialog->getEndTime();

        reminderDate =addEventDialog->getReminderDate();
        remDay=reminderDate.day();
        remMonth=reminderDate.month();
        remYear=reminderDate.year();
        remTime=addEventDialog->getReminderTime();
        //Add new appointment to database (id autogenerated)
        if (dbm.isOpen())
        {
            qDebug()<<"adding appointment..";
            dbm.addAppointment(title, location, day, month, year,
                               startTime, endTime,
                               remDay,remMonth,remYear,remTime);
        }
        showDbEventsAll();
        ui->listView->setModel(&eventModel); //?
    }
}

void MainWindow::on_actionShow_All_triggered()
{
     showDbEventsAll();
}
