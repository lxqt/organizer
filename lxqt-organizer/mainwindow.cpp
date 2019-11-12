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
    dbm.createDatebaseTables();
    QMainWindow::centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    //setup calendar
    ui->calendarWidget->setGridVisible(false); //no grid
    ui->actionCalendar_Grid->setChecked(false);
    ui->calendarWidget->setVerticalHeaderFormat(ui->calendarWidget->NoVerticalHeader);
    //setup tableview
    ui->tableView->horizontalHeader()->setVisible(true);
    ui->tableView->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setVisible(false);

    ui->tableViewContacts->horizontalHeader()->setVisible(true);
    ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewContacts->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewContacts->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewContacts->verticalHeader()->setVisible(false);

    selectedDate=QDate::currentDate();
    ui->labelSchedule->setText("Schedule for "+selectedDate.toString());

    //setup status bar
    statusTimeLabel= new QLabel();
    statusTimeLabel->setText(QTime::currentTime().toString(Qt::SystemLocaleDate));
    statusTimeLabel->setText(QTime::currentTime().toString("hh:mm"));
    statusDateLabel = new QLabel(this);
    statusDateLabel->setText(QDate::currentDate().toString());
    ui->statusBar->addPermanentWidget(statusDateLabel);
    ui->statusBar->addPermanentWidget(statusTimeLabel);
    ui->statusBar->showMessage("Hello. Watch for messages here! Double click items to delete",5000);

    //setup timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdateSlot()));
    timer->start(1000); //check for reminders every 30000ms or30 seconds
    //pause timer when reminder QTimer::stop()

    //setup models
    //day appointments
    dayModel = new AppointmentModel(); //stores appointments for a day
    proxyModel = new ProxyModel; //needed for time sorting
    //contact model
    contactModel = new ContactModel; //stores all contacts
    proxyModelContacts= new ProxyModelContacts;
    //reminder model
    reminderModel = new ReminderModel(); //stores reminders

    setCalendarOptions();

    //Load Lists
    LoadContactsListFromDatabase();
    DisplayContactsOnTableView();
    LoadAppointmentsListFromDatabase();
    showAllEventsOnCalendar();
    DisplayEventsForDate(this->selectedDate); //appointments and birthdays

    ui->actionSystem_Notifications->setChecked(true);
    this->notificationsFlag=true;
     checkForReminders();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddAppointment()
{
    DialogAddAppointment *addAppointmentDialog = new  DialogAddAppointment(this,&selectedDate);
    addAppointmentDialog->setModal(true);

    if (addAppointmentDialog->exec() == QDialog::Accepted ) {

        //qDebug()<<"get Appointment data";
        title =addAppointmentDialog->getTitle();
        location =addAppointmentDialog->getLocation();
        description= addAppointmentDialog->getDescription();
        selectedDate=addAppointmentDialog->getAppointmentDate();
        appointmentStartTime=addAppointmentDialog->getStartTime();
        appointmentEndTime=addAppointmentDialog->getEndTime();
        appointmentStartDateTime.setDate(selectedDate);
        appointmentStartDateTime.setTime(appointmentStartTime);
        appointmentStartTimestamp = appointmentStartDateTime.toString();        
        appointmentEndDateTime.setDate(selectedDate);
        appointmentEndDateTime.setTime(appointmentEndTime);
        appointmentEndTimestamp = appointmentEndDateTime.toString();        
        reminderRequested =addAppointmentDialog->getReminderRequested();       
        reminderDate=addAppointmentDialog->getReminderDate();
        reminderTime=addAppointmentDialog->getReminderTime();
        reminderDateTime.setDate(reminderDate);
        reminderDateTime.setTime(reminderTime);
        reminderTimestamp=reminderDateTime.toString();
        Appointment tmp;
        tmp.m_title=title;
        tmp.m_location=location;
        tmp.m_description=description;
        tmp.m_appointmentStartTimestamp=appointmentStartTimestamp;
        tmp.m_appointmentEndTimestamp=appointmentEndTimestamp;
        tmp.m_reminderRequest=reminderRequested;
        tmp.m_reminderTimestamp=reminderTimestamp;

        if (dbm.isOpen())
        {
            //qDebug()<<"attempting to add appointment to database and QList..";
            if(dbm.addAppointment(tmp))
            {
                appointmentsList.append(tmp);
                //qDebug()<<"appointment added to database and appointment list";
            }
        }
    }
    DisplayEventsForDate(selectedDate); //updates the appointments list
}

void MainWindow::AddContact()
{
    DialogAddContact *addContactDialog = new  DialogAddContact(this, &selectedDate);
    addContactDialog->setModal(true);

    if (addContactDialog->exec() == QDialog::Accepted ) {
        this->contactFirstName=addContactDialog->getFirstName();
        this->contactLastName=addContactDialog->getLastName();
        this->contactEmail=addContactDialog->getEmail();
        this->birthdayDate=addContactDialog->getBirthday();
        this->birthdayReminderDate=addContactDialog->getReminderDate();
        this->birthdayReminderTime=addContactDialog->getReminderTime();
        this->birthdayReminderRequest=addContactDialog->getReminderRequested();
        this->street=addContactDialog->getStreet();
        this->city=addContactDialog->getCity();
        this->county=addContactDialog->getCounty();
        this->postcode=addContactDialog->getPostcode();
        this->country=addContactDialog->getCountry();
        this->phoneNumber=addContactDialog->getPhoneNumber();
        birthdayDateTime.setDate(birthdayDate);
        birthdayDateTime.setTime(QTime(0,0,0,1)); //just after midnight
        this->birthdayTimestamp=birthdayDateTime.toString();

        this->birthdayReminderDateTime.setDate(birthdayReminderDate);
        this->birthdayReminderDateTime.setTime(birthdayReminderTime);
        this->birthdayReminderTimestamp=birthdayReminderDateTime.toString();

        Contact c;
        c.m_firstname=contactFirstName;
        c.m_lastname=contactLastName;
        c.m_email=contactEmail;
        c.m_street=street;
        c.m_city=city;
        c.m_county=county;
        c.m_postcode=postcode;
        c.m_country=country;
        c.m_telephone=phoneNumber;
        c.m_birthdayTimestamp=birthdayTimestamp;
        c.m_reminderRequest=birthdayReminderRequest;
        c.m_reminderTimestamp=birthdayReminderTimestamp;

        if (dbm.isOpen())
        {
            if(dbm.addContact(c)) //add contact to database
            {
                //qDebug()<<"contact added to database";
                contactsList.append(c); //update contacts list
            }
        }
    }
    LoadContactsListFromDatabase(); //refresh
    DisplayContactsOnTableView();
    DisplayEventsForDate(selectedDate);
}

void MainWindow::LoadAppointmentsListFromDatabase()
{
    appointmentsList.clear();
    QList<Appointment> tmpList = dbm.getAllAppointments();
    foreach(Appointment a, tmpList)
    {
        appointmentsList.append(a);
    }
}

void MainWindow::LoadContactsListFromDatabase()
{
    contactsList.clear();
    QList<Contact> tmpList = dbm.getAllContacts();
    foreach(Contact c, tmpList)
    {
        contactsList.append(c);
    }
}

void MainWindow::DisplayContactsOnTableView()
{
    contactModel->clearAllContacts();

    foreach(Contact c, contactsList)
    {
        //qDebug()<<"Contact name = "<<c.m_firstname<<" "<<c.m_lastname;
        contactModel->AddContact(c);
    }
    proxyModelContacts->setSourceModel(contactModel);
    ui->tableViewContacts->setModel(proxyModelContacts);
    ui->tableViewContacts->setSortingEnabled(true);
    ui->tableViewContacts->sortByColumn(2, Qt::AscendingOrder); //sort on lastname
    ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewContacts->setSortingEnabled(false);
    ui->tableViewContacts->hideColumn(0); //index
    ui->tableViewContacts->hideColumn(4); //street
    ui->tableViewContacts->hideColumn(5); //city
    ui->tableViewContacts->hideColumn(6); //county
    ui->tableViewContacts->hideColumn(7); //postcode
    ui->tableViewContacts->hideColumn(8); //country
    ui->tableViewContacts->hideColumn(9); //telephone
}

void MainWindow::UpdateDisplayList()
{
    //qDebug()<<"Updating display list....";

    displayList.clear();
    //load appointments
    foreach(Appointment a, appointmentsList)
    {
        displayList.append(a);
    }
    //load birthdays
    //Change to add birthdays starting from current year for say 100 years
    QDate today =QDate::currentDate();
    int currentYear =today.year();
    //qDebug()<<"Current year = "<<currentYear;

    foreach(Contact c, contactsList)
    {
        Appointment birthdayAppointment; //special appointment
        birthdayAppointment.m_title="Birthday: "+c.m_firstname+" "+c.m_lastname;
        birthdayAppointment.m_location=c.m_city;
        birthdayAppointment.m_description="Birthday";       
        QDateTime birthDateTime = QDateTime::fromString(c.m_birthdayTimestamp);

        QDate bornDate =birthDateTime.date();

        //Birthday this current year (more to do for future years (recurring)
        QDateTime birthdayThisYear;
        birthdayThisYear.setDate(QDate(currentYear,
                                      bornDate.month(),
                                      bornDate.day()));

        birthdayThisYear.setTime(QTime(0,0,0,1));
        birthdayAppointment.m_appointmentStartTimestamp=birthdayThisYear.toString();
        birthdayThisYear.setTime(QTime(23,59,0,0));
        birthdayAppointment.m_appointmentEndTimestamp=birthdayThisYear.toString();
        birthdayAppointment.m_reminderRequest=c.m_reminderRequest;
        birthdayAppointment.m_reminderTimestamp=c.m_reminderTimestamp;
        displayList.append(birthdayAppointment);
    }
}

void MainWindow::DisplayEventsForDate(QDate theSelectedDate)
{
     dayModel->clearAllAppointments();//clear model
     UpdateDisplayList(); //with appointments and birthdays
     foreach(Appointment a, displayList)
     {
         QDateTime startdatetime =QDateTime::fromString(a.m_appointmentStartTimestamp);
         QDate eventDate =startdatetime.date();

         if (eventDate ==theSelectedDate)
         {
             dayModel->addAppointment(a);
         }
     }
     proxyModel->setSourceModel(dayModel);
     ui->tableView->setModel(proxyModel);
     ui->tableView->setSortingEnabled(true);
     ui->tableView->sortByColumn(2, Qt::AscendingOrder);//minutes
     ui->tableView->sortByColumn(1, Qt::AscendingOrder); //hour
     ui->tableView->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
     ui->tableView->setSortingEnabled(false);
     ui->tableView->hideColumn(0); //appointmentID
     ui->tableView->hideColumn(1); //sort hour
     ui->tableView->hideColumn(2); //sort min
     ui->tableView->hideColumn(3);
     showAllEventsOnCalendar();
}

void MainWindow::setCalendarOptions()
{
    ui->calendarWidget->setGridVisible(false);
    ui->calendarWidget->setVerticalHeaderFormat(ui->calendarWidget->NoVerticalHeader);
    eventDayFormat.setForeground(Qt::magenta);
    eventDayFormat.setBackground(Qt::gray);
    weekDayFormat.setForeground(Qt::black);
    weekDayFormat.setBackground(Qt::white);
    weekendFormat.setForeground(Qt::red);
    weekendFormat.setBackground(Qt::white);
}

void MainWindow::showAllEventsOnCalendar()
{
    UpdateDisplayList(); //contains current appointments and birthdays
    foreach(Appointment a, displayList)
    {
        QDateTime startdatetime =QDateTime::fromString(a.m_appointmentStartTimestamp);
        QDate eventDate =startdatetime.date();
        ui->calendarWidget->setDateTextFormat(eventDate,eventDayFormat);
    }
}

void MainWindow::clearAllEventsOnCalendar()
{
    UpdateDisplayList(); //contains current appointments and birthdays
    foreach(Appointment a, displayList)
    {
        QDateTime startdatetime =QDateTime::fromString(a.m_appointmentStartTimestamp);
        QDate eventDate =startdatetime.date();

        if(eventDate.dayOfWeek()==Qt::Saturday || eventDate.dayOfWeek()==Qt::Sunday)
        {
            ui->calendarWidget->setDateTextFormat(eventDate,weekendFormat);
        }
        else {
            ui->calendarWidget->setDateTextFormat(eventDate,weekDayFormat);
        }
    }
}

void MainWindow::checkForReminders()
{
    QDate currentDate=QDate::currentDate();
    QTime currentTime= QTime::currentTime();
    int currentHour =currentTime.hour();
    int currentMinute = currentTime.minute();
    int currentSecond =currentTime.second();

    foreach(Appointment a, appointmentsList){

        QDateTime reminderdatetime =QDateTime::fromString(a.m_reminderTimestamp);
        QDate reminderDate =reminderdatetime.date();
        QTime reminderTime =reminderdatetime.time();
        int reminderHour =reminderTime.hour();
        int reminderMinute = reminderTime.minute();

        if (reminderDate==currentDate)
        {
            if(currentHour==reminderHour && currentMinute==reminderMinute &&currentSecond==0 ){
                //fire single shot timer
                reminderAppointment=a;
                QTimer::singleShot(2000, this, SLOT(sendReminderMessage()));
            }
        }
    }
}

void MainWindow::timerUpdateSlot()
{
    QTime currentTime =QTime::currentTime();
    //statusTimeLabel->setText(currentTime.toString("hh:mm"));
    statusTimeLabel->setText(currentTime.toString("hh:mm:ss"));
    checkForReminders();
}

void MainWindow::sendReminderMessage()
{
    ui->statusBar->showMessage("New reminder .. ",10000); //10 seconds
    reminderModel->addReminderAppointment(reminderAppointment);
    if (notificationsFlag){
        QString h="Reminder: "+reminderAppointment.m_title +" ("+reminderAppointment.m_location+")";
        QDateTime startdatetime =QDateTime::fromString(reminderAppointment.m_appointmentStartTimestamp);
        QDate startDate =startdatetime.date();
        QTime startTime =startdatetime.time();
        int hour =startTime.hour();
        int min =startTime.minute();
        QString m="Date: "+startDate.toString()
                + " Starts at "+QString::number(hour)+":"+QString::number(min);
        MessageData *msg = new MessageData(h,m);
        DbusSessionMessage *dbus = new DbusSessionMessage(this);
        dbus->displayMessage(*msg);
    }
ui->listViewReminders->setModel(reminderModel);
}

void MainWindow::on_actionNew_Appointment_triggered()
{
    AddAppointment();
}

void MainWindow::on_actionNew_Contact_triggered()
{
    AddContact();
}

void MainWindow::on_actionDelete_All_Contacts_triggered()
{
    dbm.removeAllContacts();
    contactsList.clear();
    contactModel->clearAllContacts();
    DisplayContactsOnTableView();
}

void MainWindow::on_actionDelete_All_Appointments_2_triggered()
{
    dbm.removeAllAppointments();
    appointmentsList.clear();
    DisplayEventsForDate(this->selectedDate); //should still display birthdays
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data(); //id in column 0
    selectedAppointmentdDbId =idd.toInt();
    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete Appointment");
    msgBox.setText("Are you sure you want to delete appointment?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
        //remove from database (but not appointment list which need reloading)
        dbm.removeAppointmentById(selectedAppointmentdDbId);

        if(selectedDate.dayOfWeek()==Qt::Saturday || selectedDate.dayOfWeek()==Qt::Sunday)
        {
            ui->calendarWidget->setDateTextFormat(selectedDate,weekendFormat);
        }
        else {
            ui->calendarWidget->setDateTextFormat(selectedDate,weekDayFormat);
        }

    }else {
       // qDebug()<<"abort delete event";
    }
    LoadAppointmentsListFromDatabase(); //reload appointments list as changed
    DisplayEventsForDate(this->selectedDate);
}

void MainWindow::on_tableViewContacts_doubleClicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data(); //id in column 0
    //qDebug()<<"Contact ID = "<<idd;
    qDebug()<<"ID Value extracted from table view = "<<idd;
    selectedContactDbId =idd.toInt();
    qDebug()<<"SelectedDbID  = "<<selectedContactDbId;

    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete Contact");
    msgBox.setText("Are you sure you want to delete contact?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
        //remove from database
        dbm.removeContactById(selectedContactDbId);
    }
    else {
        qDebug()<<"abort delete";
    }
    LoadContactsListFromDatabase(); //clears and reloads contact lsit
    DisplayContactsOnTableView();
    DisplayEventsForDate(this->selectedDate); //calls update display list
}

void MainWindow::on_tableViewContacts_clicked(const QModelIndex &index)
{

    qDebug()<<"Row index = "<<index.row();
    selectedContact =contactModel->getContact(index.row()+1);
    qDebug()<<"Selected contact email = "<<selectedContact.m_email;

    QVariant idd = index.sibling(index.row(),0).data(); //id in column 0
    //qDebug()<<"Contact ID = "<<idd;
    qDebug()<<"ID Value extracted from table view = "<<idd;
    selectedContactDbId =idd.toInt();
    qDebug()<<"SelectedDbID  = "<<selectedContactDbId;
}

void MainWindow::on_pushButtonShowQuickDetails_clicked()
{
   if (quickViewFlag)
   {
    ui->tableViewContacts->hideColumn(0); //index
    ui->tableViewContacts->hideColumn(4); //street
    ui->tableViewContacts->hideColumn(5); //city
    ui->tableViewContacts->hideColumn(6); //county
    ui->tableViewContacts->hideColumn(7); //postcode
    ui->tableViewContacts->hideColumn(8); //country
    ui->tableViewContacts->hideColumn(9); //telephone
    quickViewFlag=false;
   }
   else {
       ui->tableViewContacts->hideColumn(0); //index
       ui->tableViewContacts->showColumn(4); //street
       ui->tableViewContacts->showColumn(5); //city
       ui->tableViewContacts->showColumn(6); //county
       ui->tableViewContacts->showColumn(7); //postcode
       ui->tableViewContacts->showColumn(8); //country
       ui->tableViewContacts->showColumn(9); //telephone
       quickViewFlag=true;
   }

}
void MainWindow::on_pushButtonMailTo_clicked()
{
    //qDebug()<<"Selected contact email = "<<selectedContact.m_email;
    QString url="mailto:?to="+selectedContact.m_email+"&subject=Enter the subject&body=Enter message";
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

void MainWindow::on_actionExport_Contacts_XML_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Xml", ".", "Xml files (*.xml)");
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        qDebug() << "Error saving XML file.";
    xml.setDevice(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("CONTACTS");
    foreach(Contact c, contactsList){
        xml.writeStartElement("Contact");
        xml.writeTextElement("FirstName", c.m_firstname );
        xml.writeTextElement("LastName", c.m_lastname);
        xml.writeTextElement("Email", c.m_email);
        xml.writeTextElement("Street", c.m_street);
        xml.writeTextElement("City", c.m_city);
        xml.writeTextElement("County", c.m_county);
        xml.writeTextElement("Postcode", c.m_postcode);
        xml.writeTextElement("Country", c.m_country);
        xml.writeTextElement("Telephone", c.m_telephone);
        xml.writeTextElement("BirthTimestamp", c.m_birthdayTimestamp);
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    selectedDate=date;
    ui->labelSchedule->setText("Schedule for "+date.toString());
    DisplayEventsForDate(selectedDate);

}

void MainWindow::on_pushButtonSearchByLastname_clicked()
{
    //To do...

}

void MainWindow::on_actionExport_Appointments_XML_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Xml", ".", "Xml files (*.xml)");
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        qDebug() << "Error saving XML file.";
    xml.setDevice(&file);

    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("APPOINTMENTS");
    foreach(Appointment a, appointmentsList){
        xml.writeStartElement("Appointment");
        xml.writeTextElement("Title", a.m_title );
        xml.writeTextElement("Location", a.m_location);
        xml.writeTextElement("Description", a.m_description);
        xml.writeTextElement("StartTimestamp", a.m_appointmentStartTimestamp);
        xml.writeTextElement("EndTimestamp", a.m_appointmentEndTimestamp);
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

}

void MainWindow::on_listViewReminders_clicked(const QModelIndex &index)
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
        reminderModel->removeReminderAppointment(selectedReminderRowIdx);
    }else {
        qDebug()<<"abort delete reminder";
    }
    //update reminder listview
    ui->listViewReminders->setModel(reminderModel);
}

void MainWindow::on_actionClear_All_Reminder_Messages_triggered()
{
    reminderModel->clearAllReminderAppointments();
    ui->listViewReminders->setModel(reminderModel);
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout *aboutDialog = new DialogAbout(this);
    aboutDialog->setModal(false);
    aboutDialog->exec();
}

void MainWindow::on_actionSystem_Notifications_triggered()
{
    if(ui->actionSystem_Notifications->isChecked())
        {
            this->notificationsFlag =true;
        }
        else {
            this->notificationsFlag=false;
    }
   // ui->actionSystem_Notifications->setChecked(true);
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

