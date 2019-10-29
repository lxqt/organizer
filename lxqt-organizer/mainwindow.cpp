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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("LXQt Organizer");
    //setup databse
    dbm.openDatabase();
    dbm.createDatabaseTable();
    //set up options
    selectedDate=QDate::currentDate();
    initialiseDates();
    setCalendarOptions();
    setDefaultOptions();
    //setup calendar
    ui->calendarWidget->setGridVisible(false); //no grid
    ui->calendarWidget->setVerticalHeaderFormat(ui->calendarWidget->NoVerticalHeader);
   //setup tableview
    ui->tableView->horizontalHeader()->setVisible(true);
    ui->tableView->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setVisible(false);
    //setup reminder treeview
    ui->tableViewReminders->horizontalHeader()->setVisible(true);
    ui->tableViewReminders->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewReminders->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewReminders->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewReminders->verticalHeader()->setVisible(false);
    //setup status bar
    statusTimeLabel= new QLabel();
    statusTimeLabel->setText(QTime::currentTime().toString());
    statusDateLabel = new QLabel(this);
    statusDateLabel->setText(QDate::currentDate().toString());
    ui->statusBar->addPermanentWidget(statusDateLabel);
    ui->statusBar->addPermanentWidget(statusTimeLabel);
    ui->statusBar->showMessage("Hello. Watch for messages here! Double click to delete",5000);
    //setup timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdateSlot()));
    timer->start(1000);
    //setup appointment models
    appointmentModel = new AppointmentModel(); //stores all appointments
    dayModel = new AppointmentModel(); //stores appointments for a day
    reminderModel = new AppointmentModel(); //stores reminders
    proxyModel = new ProxyModel; //needed for time sorting
    //Get data from database and display
    loadAppointmentModelFromDatabase(); //loads all apointments
    showAllEventsOnCalendar(); //shows all appointments on calendar
    showDayAppointmentsForDate(selectedDate); //show appointments for today
    ui->labelSchedule->setText("Schedule for "+selectedDate.toString());
    checkForReminders();
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
        //initialise reminder date
        reminderDays=0;
        reminderDate=selectedDate.addDays(-reminderDays);
        remDay=reminderDate.day();
        remMonth=reminderDate.month();
        remYear=reminderDate.year();
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

void MainWindow::loadAppointmentModelFromDatabase()
{
    //We need all appointments to display on calendar
    appointmentModel->clearAllAppointments();//clear model
    //get appointments stored in database as a QList
    QList<Appointment> list=dbm.getAllAppointments();
    foreach(Appointment a, list){
        Appointment *atmp = new Appointment
                (a.m_id,
                 a.m_title,
                 a.m_location,
                 a.m_day,
                 a.m_month,
                 a.m_year,
                 a.m_startTime,
                 a.m_endTime,
                 a.m_reminderDay,
                 a.m_reminderMonth,
                 a.m_reminderYear,
                 a.m_reminderTime);
           //qDebug()<<"Database id = "<<a.id;
        appointmentModel->addAppointment(*atmp);
    }
}

void MainWindow::showDayAppointmentsForDate(QDate theSelectedDate)
{
    dayModel->clearAllAppointments();//clear model
        QList<Appointment> list=dbm.getAllAppointments();
    foreach(Appointment a, list){

        QDate evtDate=QDate(a.m_year,a.m_month,a.m_day);
        QDate checkDate =QDate(theSelectedDate);

        if (evtDate ==checkDate)
        {
            Appointment *atmp = new Appointment
                    (a.m_id,
                     a.m_title,
                     a.m_location,
                     a.m_day,
                     a.m_month,
                     a.m_year,
                     a.m_startTime,
                     a.m_endTime,
                     a.m_reminderDay,
                     a.m_reminderMonth,
                     a.m_reminderYear,
                     a.m_reminderTime);

            dayModel->addAppointment(*atmp);
        }
    }//foreach

    proxyModel->setSourceModel(dayModel);
    ui->tableView->setModel(proxyModel);
    //enable sorting then sort then disable sorting
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSortingEnabled(false);
    ui->tableView->hideColumn(0); //int time for sorting
    ui->tableView->hideColumn(1); //database id
    ui->tableView->hideColumn(2); //date string
}

void MainWindow::showAllEventsOnCalendar()
{
    for(int i =0; i<appointmentModel->rowCount(); i++)
    {
        Appointment a =appointmentModel->getAppointment(i);
        QDate tmp=QDate(a.m_year,a.m_month,a.m_day);
        ui->calendarWidget->setDateTextFormat(tmp,eventDayFormat);
    }
}

void MainWindow::clearAllEventsOnCalendar()
{
    for(int i =0; i<appointmentModel->rowCount(); i++)
    {
        Appointment a =appointmentModel->getAppointment(i);

        QDate tmp=QDate(a.m_year,a.m_month,a.m_day);

        if(tmp.dayOfWeek()==Qt::Saturday || tmp.dayOfWeek()==Qt::Sunday)
        {
            ui->calendarWidget->setDateTextFormat(tmp,weekendFormat);
        }
        else {
            ui->calendarWidget->setDateTextFormat(tmp,weekDayFormat);
        }

    }
}

void MainWindow::AddAppointment()
{
    DialogAddAppointment *addAppointmentDialog = new  DialogAddAppointment(this,&selectedDate);
    addAppointmentDialog->setModal(true);

    if (addAppointmentDialog->exec() == QDialog::Accepted ) {

        title =addAppointmentDialog->getTitle();
        location =addAppointmentDialog->getLocation();

        selectedDate=addAppointmentDialog->getAppointmentDate();
        day=selectedDate.day();
        month =selectedDate.month();
        year =selectedDate.year();
        startTime=addAppointmentDialog->getStartTime();
        endTime=addAppointmentDialog->getEndTime();

        reminderDate =addAppointmentDialog->getReminderDate();
        remDay=reminderDate.day();
        remMonth=reminderDate.month();
        remYear=reminderDate.year();
        remTime=addAppointmentDialog->getReminderTime();

        if (dbm.isOpen())
        {
            qDebug()<<"adding appointment..";
            if(dbm.addAppointment(title, location, day, month, year,
                                  startTime, endTime,
                                  remDay,remMonth,remYear,remTime))
            {            
                loadAppointmentModelFromDatabase();
                QDate d=QDate(year,month,day);
                showDayAppointmentsForDate(d);
                showAllEventsOnCalendar();
            }
        }

    }
    else {
        //cancel
    }
}

void MainWindow::checkForReminders()
{
    QDate currentDate=QDate::currentDate();
    QTime currentTime= QTime::currentTime();
    int currentHour =currentTime.hour();
    //reminderModel->clearAllAppointments();

    QList<Appointment> list=dbm.getAllAppointments();
    foreach(Appointment a, list){

        if (reminderDate==currentDate)
        {

            if(currentHour==a.m_reminderTime){
                reminderModel->addAppointment(a);
                ui->statusBar->showMessage("Event reminder!!",5000); //5 seconds
                if (notificationsFlag){
                    QString h="Reminder: "+a.m_title +" ("+a.m_location+")";
                    QDate tmp =QDate(a.m_year,a.m_month,a.m_day);
                    QString m="Date: "+tmp.toString()+ " Starts at "+QString::number(a.m_startTime)+":00";
                    MessageData *msg = new MessageData(h,m);
                    DbusSessionMessage *dbus = new DbusSessionMessage(this);
                    dbus->displayMessage(*msg);
                }
            }
        }
    }
    ui->tableViewReminders->setModel(reminderModel);
    ui->tableViewReminders->hideColumn(0); //start time as int
    ui->tableViewReminders->hideColumn(1); //id
 }

void MainWindow::timerUpdateSlot()
{
    QTime currentTime =QTime::currentTime();
    statusTimeLabel->setText(currentTime.toString());

    //Check for reminders on the hour

    int hour = currentTime.hour();
    int minute = currentTime.minute();
    int second =currentTime.second();

    for (int i=0; i<24; i++) //Check 24 hour clock
    {
        if (hour ==i){
             if((minute ==0) && (second ==0))
            {
                qDebug()<<"Minute and seconds are zero checking reminders on the hour";
                checkForReminders();
            }
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAdd_Appointment_triggered()
{
    AddAppointment();
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout *aboutDialog = new DialogAbout(this);
    aboutDialog->setModal(false);
    aboutDialog->exec();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    selectedDate=date;
    ui->labelSchedule->setText("Schedule for "+date.toString());    
    showDayAppointmentsForDate(selectedDate);
}

void MainWindow::on_actionDelete_All_triggered()
{
    clearAllEventsOnCalendar(); //while there are events
    dbm.removeAllAppointments(); //remove all from database
    appointmentModel->clearAllAppointments(); //remove all from event model
    dayModel->clearAllAppointments();  //remove all from day model
    reminderModel->clearAllAppointments();
    loadAppointmentModelFromDatabase(); //should be nothing
    ui->tableViewReminders->setModel(reminderModel);
    ui->tableViewReminders->hideColumn(0);
    ui->tableViewReminders->hideColumn(1);
    ui->calendarWidget->setSelectedDate(QDate::currentDate());   
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),1).data(); //id in column 1
    qDebug()<<"ID Value extracted from table view = "<<idd;

    selectedDbId =idd.toInt();
    qDebug()<<"SelectedDbID  = "<<selectedDbId;

    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete Appointment");
    msgBox.setText("Are you sure you want to delete appointment?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
        //remove from database
        dbm.removeAppointmentById(selectedDbId);

        if(selectedDate.dayOfWeek()==Qt::Saturday || selectedDate.dayOfWeek()==Qt::Sunday)
        {
            ui->calendarWidget->setDateTextFormat(selectedDate,weekendFormat);
        }
        else {
            ui->calendarWidget->setDateTextFormat(selectedDate,weekDayFormat);
        }

    }else {
        qDebug()<<"abort delete event";
    }

    loadAppointmentModelFromDatabase();
    showDayAppointmentsForDate(selectedDate);

}

void MainWindow::on_tableViewReminders_doubleClicked(const QModelIndex &index)
{
   int selectedReminderRowIdx=index.row();
    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete Reminder");
    msgBox.setText("Are you sure you want to delete reminder?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
        //remove from model
        reminderModel->removeAppointment(selectedReminderRowIdx);
    }else {
        qDebug()<<"abort delete reminder";
    }
    //update reminder tableview
    ui->tableViewReminders->setModel(reminderModel);
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
