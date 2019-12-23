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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("LXQt Organizer");
    QMainWindow::centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    //Setup database
    dbm.openDatabase();
    dbm.createDatebaseTables();

    selectedDate=QDate::currentDate();
    ui->labelSelectedDate->setText(selectedDate.toString());

    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setRowCount(rowCount);
    ui->tableWidget->setMinimumSize(800,400);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(90);
    ui->tableWidget->verticalHeader()->setMinimumSectionSize(90);
    ui->tableWidget->verticalHeader()->setVisible(false);
    //ui->tableWidget->setLineWidth(2);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::SelectedClicked);
    //ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
   // ui->tableWidget->setStyleSheet("font()-size: 12pt;");
    firstDay = Qt::Monday;

    selectedMonth=selectedDate.month();
    selectedYear=selectedDate.year();

    QString monthName=monthNames[selectedMonth-1];
    QString monthYear=monthName+" "+QString::number(selectedYear);
    ui->labelMonthYear->setText(monthYear);

    //setup status bar
    statusTimeLabel= new QLabel();
    statusTimeLabel->setText(QTime::currentTime().toString(Qt::SystemLocaleDate));
    statusTimeLabel->setText(QTime::currentTime().toString("hh:mm"));
    statusDateLabel = new QLabel(this);
    statusDateLabel->setText(QDate::currentDate().toString());
    ui->statusBar->addPermanentWidget(statusDateLabel);
    ui->statusBar->addPermanentWidget(statusTimeLabel);
    ui->statusBar->showMessage("Hello. Welcome to LXQt Organizer (v004 alpha1)",5000);

    //setup timer
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this, SLOT(timerUpdateSlot()));
    timer->start(1000);
    ui->actionCheck_Reminders_Hourly->setChecked(true);

    ui->tableViewContacts->horizontalHeader()->setVisible(true);
    ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewContacts->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewContacts->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewContacts->verticalHeader()->setVisible(false);

    ui->tableViewDays->horizontalHeader()->setVisible(true);
    ui->tableViewDays->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewDays->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewDays->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewDays->verticalHeader()->setVisible(false);


    //Appointment ArrayList

    appointmentList= QList<Appointment>(); //empty

    //Add holidays to appointmentList
    AddHolidaysToAppointmentsList(selectedYear);
    UpdateCalendar();

    //Add database appointments to appointmentList

    dayModel= new DayModel; //model view
    proxyModelAppointments = new ProxyModelAppointments;

    LoadDatabaseAppointmentsToAppointmentList();
    LoadDatabaseRemindersToRemindersList();
    UpdateCalendar();

    //Add contacts to contactList
    contactModel = new ContactModel; //model view
    proxyModelContacts= new ProxyModelContacts;
    LoadDatebaseContactsToContactList(); //loads contactlist
    DisplayContactsOnTableView();

    AddBirthdaysToAppointmentList(selectedYear);
    UpdateCalendar();
    ShowDayAppointmentsTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::NewAppointment()
{
    DialogAppointment *appointmentDialog = new  DialogAppointment(this,&selectedDate);
    appointmentDialog->setModal(true);

    if (appointmentDialog->exec() == QDialog::Accepted ) {

        reminderDays=0;
        title =appointmentDialog->getTitle();
        location =appointmentDialog->getLocation();
        description= appointmentDialog->getDescription();
        selectedDate=appointmentDialog->getAppointmentDate(); //appointment date
        appointmentStartTime=appointmentDialog->getStartTime();
        appointmentEndTime=appointmentDialog->getEndTime();
        category=appointmentDialog->getCategory();
        reminderDays=appointmentDialog->getReminderDays();
        reminderRequested=appointmentDialog->getReminderRequested();
        isAllDay=appointmentDialog->getAllDay();

        Appointment a;
        a.m_title=title;
        a.m_location=location;
        a.m_description=description;
        a.m_date=selectedDate.toString();
        a.m_startTime=appointmentStartTime.toString();
        a.m_endTime=appointmentEndTime.toString();
        a.m_category=category;
        a.m_isFullDay=isAllDay;
        a.m_isRepeating=0;
        a.m_parentId=0;
        a.m_hasReminder=reminderRequested;

        if (dbm.isOpen())
        {
            appointmentId=dbm.addAppointment(a);
            //a.m_id=appointmentId;
            //dayModel->AddAppointment(a);
            //appointmentList.insert(appointmentId,a);
            UpdateAppointmentList();
            qDebug()<<"Added Appointment: Appointment ID = "<<appointmentId;
            ShowDayAppointmentsTableView();
        }

        //Create a reminder in everycase so that it can be editied
        QString dateStr="("+QString::number(selectedDate.day())+"/"
                +QString::number(selectedDate.month())+"/"
                +QString::number(selectedDate.year())+")";

        QString reminderDetails=title
                +"\n"+location
                + "\n"+dateStr;

        if(isAllDay==1)
        {
            reminderDetails.append(" All day event");
        }
        else {
            QString minutesStartStr = QString("%1").arg(appointmentStartTime.minute(), 2, 10, QChar('0'));
            QString startTimeStr=QString::number(appointmentStartTime.hour(),'f',0)
                    +":"+minutesStartStr;
            QString minutesEndStr = QString("%1").arg(appointmentEndTime.minute(), 2, 10, QChar('0'));
            QString endTimeStr=QString::number(appointmentEndTime.hour(),'f',0)+
                    ":"+minutesEndStr;
            reminderDetails.append(" ("+startTimeStr+" to "+endTimeStr+")");
        }

        Reminder r;

        r.m_details=reminderDetails;
        r.m_appointmentId=appointmentId; //the link
        r.m_reminderDate=selectedDate.addDays(-reminderDays).toString();
        r.m_reminderTime=appointmentStartTime.toString();;

        if (dbm.isOpen())
        {
            bool success =dbm.addReminder(r);
            //reminderList.append(r);
            UpdateReminderList();
            if (success){
                qDebug()<<"Reminder added with appointmentId = "<<appointmentId;
            }
            else {
                qDebug()<<"Reminder add failed!";
            }
        }
    }

    UpdateCalendar();
    ShowDayAppointmentsTableView();

}

void MainWindow::UpdateAppointment(int dbID)
{
    Appointment currentAppointment = dbm.getAppointmentByID(dbID);

    if (currentAppointment.m_category=="Birthday") return;

    if (currentAppointment.m_category=="Holiday") return; //Holidays handled separately

    DialogAppointment *appointmentDialog =
            new  DialogAppointment(this,&currentAppointment);
    appointmentDialog->setModal(true);

    if (appointmentDialog->exec() == QDialog::Accepted ) {
        //qDebug()<<"Delete request = "<<appointmentDialog->getDeleteRequested();

        if(appointmentDialog->getDeleteRequested())
        {

                //int reminderKey = currentAppointment.m_reminderId;
            dbm.deleteReminderById(currentAppointment.m_id);
            //RemoveReminderFromReminderList(currentAppointment.m_id);


            dbm.deleteAppointmentById(dbID);
           // RemoveAppointmentFromAppointmentList(dbID);

            //Deleting appointments from DB seems to change structure
            // so reload (update) lists using DB records

            UpdateAppointmentList();
            UpdateReminderList();

            UpdateCalendar();
            ShowDayAppointmentsTableView();
            return;
        }

        title =appointmentDialog->getTitle();
        location =appointmentDialog->getLocation();
        description= appointmentDialog->getDescription();
        selectedDate=appointmentDialog->getAppointmentDate(); //appointment date
        appointmentStartTime=appointmentDialog->getStartTime();
        appointmentEndTime=appointmentDialog->getEndTime();
        category=appointmentDialog->getCategory();
        reminderRequested=appointmentDialog->getReminderRequested();
        reminderDays=appointmentDialog->getReminderDays();
        isAllDay=appointmentDialog->getAllDay();

        Appointment a;
        a.m_title=title;
        a.m_location=location;
        a.m_description=description;
        a.m_date=selectedDate.toString();
        a.m_startTime=appointmentStartTime.toString();
        a.m_endTime=appointmentEndTime.toString();
        a.m_category=category;
        a.m_isFullDay=isAllDay;
        a.m_hasReminder =reminderRequested;
        a.m_isRepeating=0;
        a.m_parentId=0;

        if (dbm.isOpen())
        {
            dbm.updateAppointment(a,dbID);
            //UpdateAppointmentInAppointmentList(a,dbID);
            UpdateAppointmentList();
            //qDebug()<<"Apointment update: success ="<<success;
        }

        //update Reminder requested
        QString dateStr="("+QString::number(selectedDate.day())+"/"
                +QString::number(selectedDate.month())+"/"
                +QString::number(selectedDate.year())+")";

        QString reminderDetails=title
                +"\n"+location
                + "\n"+dateStr;

        if(isAllDay==1)
        {
            reminderDetails.append(" All day event");
        }
        else {
            QString minutesStartStr = QString("%1").arg(appointmentStartTime.minute(), 2, 10, QChar('0'));
            QString startTimeStr=QString::number(appointmentStartTime.hour(),'f',0)
                    +":"+minutesStartStr;
            QString minutesEndStr = QString("%1").arg(appointmentEndTime.minute(), 2, 10, QChar('0'));
            QString endTimeStr=QString::number(appointmentEndTime.hour(),'f',0)+
                    ":"+minutesEndStr;
            reminderDetails.append(" ("+startTimeStr+" to "+endTimeStr+")");
        }

        Reminder r;

        r.m_details=reminderDetails;
        r.m_reminderDate=selectedDate.addDays(-reminderDays).toString();
        r.m_reminderTime=appointmentStartTime.toString();

        if (dbm.isOpen())
        {

            dbm.updateReminder(r,dbID);
            //UpdateReminderInReminderList(r,dbID);
            UpdateReminderList();
            //qDebug()<<"Reminder update success = "<<success;
        }

        ShowDayAppointmentsTableView();
        UpdateCalendar();
    }
}

void MainWindow::LoadDatabaseAppointmentsToAppointmentList()
{
    //initialisation - do at startup
    //appointmentList.clear();
    QList<Appointment> tmpList = dbm.getAllAppointments();
    foreach(Appointment a, tmpList)
    {
        appointmentList.append(a);
    }
}

void MainWindow::UpdateAppointmentInAppointmentList(Appointment app, int appointmentId)
{
    for(int i=0; i<appointmentList.count(); i++)
    {
        Appointment a =appointmentList.at(i);

        if(a.m_id==appointmentId)
        {
            appointmentList.removeAt(i);
            appointmentList.insert(i,app);
        }
    }

}

void MainWindow::RemoveAppointmentFromAppointmentList(int appointmentId)
{
    for(int i=0; i<appointmentList.count(); i++)
    {
        Appointment a =appointmentList.at(i);

        if (a.m_id ==appointmentId)
        {
            appointmentList.removeAt(i);
        }
    }
}

void MainWindow::UpdateAppointmentList()
{

    appointmentList.clear();
    AddHolidaysToAppointmentsList(selectedYear);
    AddBirthdaysToAppointmentList(selectedYear);

    QList<Appointment> tmpList = dbm.getAllAppointments();
    foreach(Appointment a, tmpList)
    {
        appointmentList.append(a);
    }

}

void MainWindow::UpdateReminderList()
{
    reminderList.clear();
    QList<Reminder> tmpList = dbm.getAllReminders();
    foreach(Reminder r, tmpList)
    {
        reminderList.append(r);
    }
}

void MainWindow::LoadDatabaseRemindersToRemindersList()
{
    reminderList.clear();
    QList<Reminder> tmpList = dbm.getAllReminders();
    foreach(Reminder r, tmpList)
    {
        reminderList.append(r);
    }
}

void MainWindow::UpdateReminderInReminderList(Reminder rem, int appointmentId)
{
    for(int i=0; i<reminderList.count(); i++)
    {
        Reminder r =reminderList.at(i);

        if(r.m_appointmentId==appointmentId)
        {
            reminderList.removeAt(i);
            reminderList.insert(i,rem);
        }
    }
}

void MainWindow::RemoveReminderFromReminderList(int appointmentId)
{
    for(int i=0; i<reminderList.count(); i++)
    {
        Reminder r =reminderList.at(i);

        if(r.m_appointmentId==appointmentId)
        {
            reminderList.removeAt(i);
        }
    }
}


void MainWindow::ShowDayAppointmentsTableView()
{

    QList<Appointment> dayList =QList<Appointment>();

//    foreach(Appointment a, appointmentList)
//    {
//        QDate adate = QDate::fromString(a.m_date);

//        if(adate==selectedDate)
//        {
//            dayList.append(a);
//        }
//    }


    for(int i=0; i<appointmentList.count(); i++)
    {

        Appointment a = appointmentList.at(i);
        QDate adate = QDate::fromString(a.m_date);

        if(adate==selectedDate)
        {
            dayList.append(a);

            if(a.m_category=="Birthday" && a.m_showBirthday ==0)
            {
                dayList.removeLast();
            }
        }
    }

    dayModel = new DayModel(dayList);
    proxyModelAppointments->setSourceModel(dayModel);
    ui->tableViewDays->setModel(proxyModelAppointments);

    ui->tableViewDays->setSortingEnabled(true);
    ui->tableViewDays->sortByColumn(1, Qt::AscendingOrder); //sort on starttime
    ui->tableViewDays->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewDays->setSortingEnabled(false);
    //ui->tableViewDays->setModel(dayModel);
    ui->tableViewDays->hideColumn(0); //index
}

void MainWindow::LoadDatebaseContactsToContactList()
{
    //initialisation - do at startup
    contactList.clear();
    QList<Contact> tmpList = dbm.getAllContacts();
    foreach(Contact c, tmpList)
    {
        contactList.append(c);
    }
}

void MainWindow::NewContact()
{
    //Add new contact
    DialogContact *contactDialog = new  DialogContact(this, &selectedDate);
    contactDialog->setModal(true);
    if (contactDialog->exec() == QDialog::Accepted ) {

        this->contactFirstName=contactDialog->getFirstName();
        this->contactMiddleNames=contactDialog->getMiddleNames();
        this->contactLastName=contactDialog->getLastName();
        this->contactEmail=contactDialog->getEmail();
        this->street=contactDialog->getStreet();
        this->city=contactDialog->getCity();
        this->district=contactDialog->getDistrict();
        this->county=contactDialog->getCounty();
        this->postcode=contactDialog->getPostcode();
        this->country=contactDialog->getCountry();
        this->phoneNumber=contactDialog->getPhoneNumber();
        this->birthDate=contactDialog->getBirthDate();
        this->addBirthdayToCalendar=contactDialog->getAddToCalendar();

        Contact c;
        c.m_firstname=contactFirstName;
        c.m_midnames=contactMiddleNames;
        c.m_lastname=contactLastName;
        c.m_email=contactEmail;
        c.m_street=street;
        c.m_district=district;
        c.m_city=city;
        c.m_county=county;
        c.m_postcode=postcode;
        c.m_country=country;
        c.m_telephone=phoneNumber;
        c.m_birthdate=birthDate.toString();
        c.m_addToCalendar=addBirthdayToCalendar;

        if (dbm.isOpen())
        {
            contactId=dbm.addContact(c);

            c.m_id=contactId;
            contactList.append(c);
            //qDebug()<<"Added contact contactId = "<<contactId;
        }

       //Don't store- dynamically create on-the-fly

       Appointment ba;
       ba.m_title="Birthday";
       ba.m_location=city;
       ba.m_description=contactFirstName+" "+contactLastName;

       QDate currentBDay =QDate(selectedDate.year(), birthDate.month(), birthDate.day());
       ba.m_date=currentBDay.toString();
       ba.m_startTime=QTime(0,0,0,1).toString();
       ba.m_isFullDay=1;
       ba.m_showBirthday=addBirthdayToCalendar;
       ba.m_category="Birthday";
       ba.m_contactId=contactId;
       appointmentList.append(ba);

    }

    DisplayContactsOnTableView(); //display   
    UpdateCalendar();
    ShowDayAppointmentsTableView();
}

void MainWindow::UpdateContact(int dbID)
{
    Contact currentContact = dbm.getContactByID(dbID);
     //qDebug()<<"currentContact ID = "<<currentContact.m_id;
    //qDebug()<<"birthday appointment ID = "<<currentContact.m_birthdayAppointmentId;
       DialogContact *contactDialog = new  DialogContact(this,&currentContact); //(this,&selectedDate);

       contactDialog->setModal(true);
       if (contactDialog->exec() == QDialog::Accepted ) {

           if(contactDialog->getDeleteRequested())
           {

               //dbm.removeBirthdayById(selectedContact.m_id);
               dbm.removeContactById(dbID);

               RemoveContactFromContactList(dbID);
               RemoveBirthdayFromAppointmentList(dbID);

               DisplayContactsOnTableView();
               UpdateCalendar();
               ShowDayAppointmentsTableView();
               return;  //gone!
           }
           this->contactFirstName=contactDialog->getFirstName();
           this->contactLastName=contactDialog->getLastName();
           this->contactMiddleNames=contactDialog->getMiddleNames();
           this->contactEmail=contactDialog->getEmail();
           this->street=contactDialog->getStreet();
           this->district=contactDialog->getDistrict();
           this->city=contactDialog->getCity();
           this->county=contactDialog->getCounty();
           this->postcode=contactDialog->getPostcode();
           this->country=contactDialog->getCountry();
           this->phoneNumber=contactDialog->getPhoneNumber();
           this->birthDate=contactDialog->getBirthDate();
           this->addBirthdayToCalendar=contactDialog->getAddToCalendar();
           this->birthDateAppointmentId=contactDialog->getBirthDateId();

           Contact c;
           c.m_firstname=contactFirstName;
           c.m_midnames=contactMiddleNames;
           c.m_lastname=contactLastName;
           c.m_email=contactEmail;
           c.m_street=street;
           c.m_district=district;
           c.m_city=city;
           c.m_county=county;
           c.m_postcode=postcode;
           c.m_country=country;
           c.m_telephone=phoneNumber;
           c.m_birthdate=birthDate.toString();           
           c.m_addToCalendar=addBirthdayToCalendar;

           if (dbm.isOpen())
           {
              dbm.updateContact(c,dbID);

           }

           Appointment ba;
           ba.m_title="Birthday";
           ba.m_location=city;
           ba.m_description=contactFirstName+" "+contactLastName;

           QDate currentBDay =QDate(selectedDate.year(), birthDate.month(), birthDate.day());
           ba.m_date=currentBDay.toString();
           ba.m_startTime=QTime(0,0,0,1).toString();
           ba.m_isFullDay=1;
           ba.m_showBirthday=addBirthdayToCalendar;
           ba.m_category="Birthday";
           ba.m_contactId=dbID;

          UpdateBirthdayInAppointmentsList(ba,dbID);
          UpdateCalendar();

       }
       DisplayContactsOnTableView();
       UpdateCalendar();
       ShowDayAppointmentsTableView();

}

void MainWindow::DisplayContactsOnTableView()
{
    contactModel->clearAllContacts();

    QList<Contact> contactListDb =dbm.getAllContacts();

    foreach(Contact c, contactListDb)
    {
        //qDebug()<<"Adding Contacts to contactModel >> "<<c.m_firstname<<" "<<c.m_lastname;
        contactModel->AddContact(c);
    }
    proxyModelContacts->setSourceModel(contactModel);
    ui->tableViewContacts->setModel(proxyModelContacts);
    ui->tableViewContacts->setSortingEnabled(true);
    ui->tableViewContacts->sortByColumn(3, Qt::AscendingOrder); //sort on lastname
    ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewContacts->setSortingEnabled(false);
    ui->tableViewContacts->hideColumn(0); //index
    ui->tableViewContacts->hideColumn(2); //mid names
    ui->tableViewContacts->hideColumn(5); //street
    ui->tableViewContacts->hideColumn(6); //district
    //city
    ui->tableViewContacts->hideColumn(8); //county
    ui->tableViewContacts->hideColumn(9); //postcode
    ui->tableViewContacts->hideColumn(10); //country
    ui->tableViewContacts->hideColumn(11); //telephone
    //birthday
    ui->tableViewContacts->hideColumn(13); //birthID

}

void MainWindow::AddBirthdaysToAppointmentList(int year)
{
    //Dynamically creating appointments from contacts

    Appointment ba;

    foreach(Contact c, contactList)
    {
        ba.m_title="Birthday";
        ba.m_location=c.m_city;
        ba.m_description=c.m_firstname+" "+c.m_lastname;

        QDate birthDate=QDate::fromString(c.m_birthdate);

        QDate currentBDay =QDate(year, birthDate.month(), birthDate.day());
        ba.m_date=currentBDay.toString();

        ba.m_isFullDay=1;
        ba.m_showBirthday=c.m_addToCalendar;
        ba.m_category="Birthday";
        ba.m_startTime=QTime(0,0,0,1).toString();
        ba.m_contactId=c.m_id;
        appointmentList.append(ba);
    }
}

void MainWindow::UpdateBirthdaysInAppointmentList(int year)
{
    for(int i=0; i<appointmentList.count(); i++)
    {
        Appointment a =appointmentList.at(i);
        Appointment tmp;
        if(a.m_category=="Birthday")
        {
            tmp.m_id=a.m_id;
            tmp.m_title=a.m_title;
            tmp.m_description=a.m_description;
            tmp.m_location=a.m_location;
            tmp.m_isFullDay=a.m_isFullDay;
            tmp.m_category=a.m_category;
            tmp.m_showBirthday=a.m_showBirthday;
            tmp.m_contactId=a.m_contactId;
            tmp.m_startTime=QTime(0,0,0,1).toString();

            QDate date =QDate::fromString(a.m_date);
            date.setDate(year,date.month(),date.day());
            tmp.m_date=date.toString();

            appointmentList.removeAt(i);
            appointmentList.insert(i,tmp);
        }
    }
}

void MainWindow::RemoveContactFromContactList(int contactId)
{
    for(int i=0; i<contactList.count(); i++)
    {
        Contact c =contactList.at(i);

        if (c.m_id ==contactId)
        {
            contactList.removeAt(i);
        }
    }
}

void MainWindow::RemoveBirthdayFromAppointmentList(int contactId)
{
    qDebug()<<"Remove birthday from appointmentList contactId = "<<contactId;

    for(int i=0; i<appointmentList.count(); i++)
    {
        Appointment a =appointmentList.at(i);

        if(a.m_contactId==contactId)
        {
            appointmentList.removeAt(i);

        }
    }
}

void MainWindow::UpdateBirthdayInAppointmentsList(Appointment b, int contactId)
{
    for(int i=0; i<appointmentList.count(); i++)
    {
        Appointment a =appointmentList.at(i);

        if(a.m_contactId==contactId)
        {
            appointmentList.removeAt(i);
            appointmentList.insert(i,b);
        }
    }
}


void MainWindow::AddHolidaysToAppointmentsList(int year)
{

    Appointment h1;
    h1.m_id=-1;
    h1.m_title="Christmas ";
    h1.m_description= "Holiday ";
    h1.m_location="Home";
    h1.m_date=QDate(year,12,25).toString();
    h1.m_isFullDay=1;
    h1.m_startTime=QTime(0,0,0,1).toString();
    h1.m_category="Holiday";
    appointmentList.append(h1);

    Appointment h2;
    h2.m_id=-2;
    h2.m_title="Boxing Day ";
    h2.m_description= "Holiday ";
    h2.m_location="Home";
    h2.m_date=QDate(year,12,26).toString();
    h2.m_isFullDay=1;
    h2.m_startTime=QTime(0,0,0,1).toString();
    h2.m_category="Holiday";
    appointmentList.append(h2);

    Appointment h3;
    h3.m_id=-3;
    h3.m_title="New Year Day ";
    h3.m_description= "Holiday ";
    h3.m_location="Home";
    h3.m_date=QDate(year,1,1).toString();
    h3.m_isFullDay=1;
    h3.m_startTime=QTime(0,0,0,1).toString();
    h3.m_category="Holiday";
    appointmentList.append(h3);

    //Calculate Easter and add
    QDate easterSunday =CalculateEaster(year);
    //qDebug()<<"Easter Sunday 2019 = "<<easterSunday.toString();

    Appointment h4;
    h4.m_id=-4;
    h4.m_title="Easter";
    h4.m_description= "Holiday ";
    h4.m_location="Home";
    h4.m_date=easterSunday.toString();
    h4.m_isFullDay=1;
    h4.m_startTime=QTime(0,0,0,1).toString();
    h4.m_category="Holiday";
    appointmentList.append(h4);

    Appointment h5;
    h5.m_id=-5;
    h5.m_title="Good Friday";
    h5.m_description= "Holiday ";
    h5.m_location="Home";
    h5.m_date=easterSunday.addDays(-2).toString();
    h5.m_isFullDay=1;
    h5.m_startTime=QTime(0,0,0,1).toString();
    h5.m_category="Holiday";
    appointmentList.append(h5);

    Appointment h6;
    h6.m_id=-6;
    h6.m_title="Easter Monday";
    h6.m_description= "Holiday ";
    h6.m_location="Home";
    h6.m_date=easterSunday.addDays(1).toString();
    h6.m_isFullDay=1;
    h6.m_startTime=QTime(0,0,0,1).toString();
    h6.m_category="Holiday";
    appointmentList.append(h6);

}

void MainWindow::UpdateHolidaysInAppointmentList(int year)
{

    for(int i=0; i<appointmentList.count(); i++)
    {
        Appointment a =appointmentList.at(i);
        Appointment tmp;
        if(a.m_category=="Holiday")
        {
            tmp.m_id=a.m_id;
            tmp.m_title=a.m_title;
            tmp.m_description=a.m_description;
            tmp.m_location=a.m_location;
            tmp.m_isFullDay=a.m_isFullDay;
            tmp.m_category=a.m_category;
            tmp.m_startTime=QTime(0,0,0,1).toString();

            if (a.m_title=="Easter")
            {
                QDate easterSunday =CalculateEaster(year);
                tmp.m_date=easterSunday.toString();
            }
            else if(a.m_title=="Good Friday")
            {
                QDate easterSunday =CalculateEaster(year);
                tmp.m_date=easterSunday.addDays(-2).toString();
            }
            else if(a.m_title=="Easter Monday")
            {
                QDate easterSunday =CalculateEaster(year);
                tmp.m_date=easterSunday.addDays(1).toString();
            }
            else {

                QDate date =QDate::fromString(a.m_date);
                date.setDate(year,date.month(),date.day());
                tmp.m_date=date.toString();

            }

            appointmentList.removeAt(i);
            appointmentList.insert(i,tmp);
        }
    }
}

void MainWindow::RemoveHolidaysFromAppointmentsList()
{

   for(int i=0; i<appointmentList.count(); i++)
   {
       Appointment a =appointmentList.at(i);

       if(a.m_category=="Holiday")
       {
           appointmentList.removeAt(i);
       }
   }
}

void MainWindow::CheckForRemindersOnHour()
{
    QDate currentDate=QDate::currentDate();
    QTime currentTime= QTime::currentTime();
    int currentHour =currentTime.hour();
    int currentMinute = currentTime.minute();
    int currentSecond =currentTime.second();

    //hourly
    if((currentHour==1 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==2 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==3 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==4 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==5 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==6 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==7 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==8 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==9 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==10 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==11 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==12 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==13 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==14 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==15 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==16 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==17 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==18 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==19 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==20 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==21 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==22 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==23 && currentMinute==0 &&currentSecond==0) ||
            (currentHour==24 && currentMinute==0 &&currentSecond==0))

    {
        //checkForBirthdaysNextSevenDays();

        foreach(Reminder r, reminderList){

            QDate reminderDate =QDate::fromString(r.m_reminderDate);

            if (reminderDate==currentDate)
            {
                QString str=r.m_details;
                QMessageBox::information(this,"Reminder",str);

                //fire single shot timer
                if (flagNotifications){
                    reminderSingleShot=r;
                    QTimer::singleShot(2000, this, SLOT(sendReminderMessage()));
                }
            }            
        }
    }
}

void MainWindow::checkForReminders()
{
    QDate currentDate=QDate::currentDate();
    foreach(Reminder r, reminderList){

        QDate reminderDate =QDate::fromString(r.m_reminderDate);

        if (reminderDate==currentDate)
        {
            QString str=r.m_details;
            QMessageBox::information(this,"Reminder",str);
        }
        //fire single shot timer
        if (flagNotifications){
            reminderSingleShot=r;
            QTimer::singleShot(2000, this, SLOT(sendReminderMessage()));
        }
    }
}

void MainWindow::DisplayRemindersForDate(QDate date)
{
    DialogShowReminders *remindersDialog =
            new  DialogShowReminders(this, &date, &dbm);
    remindersDialog->setModal(true);

    if (remindersDialog->exec() == QDialog::Accepted ) {
       //display reminders
    }

     if (flagNotifications){

         foreach(Reminder r, reminderList){

             QDate reminderDate =QDate::fromString(r.m_reminderDate);

             if (reminderDate==date)
             {
                 reminderSingleShot=r;
                 QTimer::singleShot(2000, this, SLOT(sendReminderMessage()));
             }
         }
     }
}

void MainWindow::checkForBirthdaysNextSevenDays()
{

    QDate currentDate =QDate::currentDate();
    for(int i=1; i<8; i++)
    {
        foreach(Contact c, contactList)  //use contacts!!
        {
            QDate bornDate = QDate::fromString(c.m_birthdate);
            QDate bdayDate=QDate(currentDate.year(),bornDate.month(),bornDate.day());
            if (bdayDate.addDays(-i)==currentDate) { //add all - day birthdays

                QString str=c.m_firstname+" "+c.m_lastname;
                str.append(" ("+bdayDate.toString()+")");
                QMessageBox::information(this,"Birthday Reminder",str);
            }
        }
    }
}

QDate MainWindow::CalculateEaster(int year)
{
    QDate easter;

    int Yr = year;
    int a = Yr % 19;
    int b = Yr / 100;
    int c = Yr % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int L = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * L) / 451;
    int month = (h + L - 7 * m + 114) / 31;
    int day = ((h + L - 7 * m + 114) % 31) + 1;

    easter.setDate(year,month,day);
    return easter;
}


void MainWindow::exportContactsXML()
{
    QDomDocument document;
    QDomElement root = document.createElement("Contacts");
    document.appendChild(root);

    QList<Contact> dbContactList =dbm.getAllContacts();
    foreach(Contact c, dbContactList){
        QString firstName=c.m_firstname;
        QString midName =c.m_midnames;
        QString lastName=c.m_lastname;
        QString email =c.m_email;
        QString street =c.m_street;
        QString district =c.m_district;
        QString city =c.m_city;
        QString county =c.m_county;
        QString postcode=c.m_postcode;
        QString country =c.m_country;
        QString telephone =c.m_telephone;
        QString birthdate =c.m_birthdate;

        QDomElement contact = document.createElement("Contact");
        contact.setAttribute("FirstName",firstName );
        contact.setAttribute("MidName",midName);
        contact.setAttribute("LastName", lastName);
        contact.setAttribute("Email",email);
        contact.setAttribute("Street",street);
        contact.setAttribute("District",district);
        contact.setAttribute("City",city);
        contact.setAttribute("County",county);
        contact.setAttribute("Postcode",postcode);
        contact.setAttribute("Country",country);
        contact.setAttribute("Telephone",telephone);
        contact.setAttribute("BirthDate",birthdate);
        root.appendChild(contact);
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save Contacts Xml", ".", "Xml files (*.xml)");
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        //qDebug() << "Error saving XML file.";
        return;
    }
    else {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        //qDebug() << "Finished";
    }

}

void MainWindow::importContactsXML()
{
    QDomDocument document;
       QString filename = QFileDialog::getOpenFileName(this, "Open Contacts Xml", ".", "Xml files (*.xml)");
       QFile file(filename);
       if (!file.open(QFile::ReadOnly | QFile::Text))
       {
           //qDebug() << "Failed to open XML file.";
           return;
       }
       else
       {
           if(!document.setContent(&file))
           {
               //qDebug() << "Failed to load document";
               return;
           }
           file.close();
       }

       QDomElement root = document.firstChildElement();
       QDomNodeList contact = root.elementsByTagName("Contact");
       for(int i = 0; i < contact.count(); i++)
       {
           QDomNode contactnode = contact.at(i);
           if(contactnode.isElement())
           {
               QDomElement contact = contactnode.toElement();

               Contact c;
               c.m_firstname=contact.attribute("FirstName");
               c.m_midnames=contact.attribute("MidName");
               c.m_lastname=contact.attribute("LastName");
               c.m_email=contact.attribute("Email");
               c.m_street=contact.attribute("Street");
               c.m_district=contact.attribute("District");
               c.m_city=contact.attribute("City");
               c.m_county=contact.attribute("County");
               c.m_postcode=contact.attribute("Postcode");
               c.m_country=contact.attribute("Country");
               c.m_telephone=contact.attribute("Telephone");
               c.m_birthdate=contact.attribute("BirthDate");

               if (dbm.isOpen())
               {
                   contactId=dbm.addContact(c);
                   qDebug()<<"Added contact contactId = "<<contactId;
               }               
           }
       }
       file.close();
       //qDebug() << "Finished";

       LoadDatebaseContactsToContactList(); //refresh contactslist here
       DisplayContactsOnTableView(); //display


      // updateCalendar();

}

void MainWindow::exportAppointmentsXML()
{
    QDomDocument document;
        QDomElement root = document.createElement("Appointments");
        document.appendChild(root);

        QList<Appointment> dbAppointmentList =dbm.getAllAppointments();
        foreach(Appointment a, dbAppointmentList){

                QString title=a.m_title;
                QString location=a.m_location;
                QString description =a.m_description;
                QString date =a.m_date;
                QString startTime =a.m_startTime;
                QString endTime=a.m_endTime;
                QString allDay =QString::number(a.m_isFullDay);
                QString category=a.m_category;

                if ((category!="Holiday") && (category!="Birthday")) {

                QDomElement appointment = document.createElement("Appointment");
                appointment.setAttribute("Title",title);
                appointment.setAttribute("Location", location);
                appointment.setAttribute("Description",description);
                appointment.setAttribute("Date",date);
                appointment.setAttribute("StartTime",startTime);
                appointment.setAttribute("EndTime",endTime);
                appointment.setAttribute("IsAllDay",allDay);
                appointment.setAttribute("Category",category);
                root.appendChild(appointment);
                }

        }

        QString filename = QFileDialog::getSaveFileName(this, "Save Appointments Xml", ".", "Xml files (*.xml)");
        QFile file(filename);
        if (!file.open(QFile::WriteOnly | QFile::Text)){

            //qDebug() << "Error saving XML file.";
            return;
        }
        else {
            QTextStream stream(&file);
            stream << document.toString();
            file.close();
            //qDebug() << "Finished";
        }
}

void MainWindow::importAppointmentsXML()
{
    QDomDocument document;
        QString filename = QFileDialog::getOpenFileName(this, "Open Appointments Xml", ".", "Xml files (*.xml)");
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            //qDebug() << "Failed to open XML file.";
            return;
        }
        else
        {
            if(!document.setContent(&file))
            {
               // qDebug() << "Failed to load document";
                return;
            }
            file.close();
        }

        QDomElement root = document.firstChildElement();
        QDomNodeList appointment = root.elementsByTagName("Appointment");
        for(int i = 0; i < appointment.count(); i++)
        {
            QDomNode appointmentnode = appointment.at(i);
            if(appointmentnode.isElement())
            {
                QDomElement appointment = appointmentnode.toElement();

                Appointment a;
                a.m_title=appointment.attribute("Title");
                a.m_location=appointment.attribute("Location");
                a.m_description=appointment.attribute("Description");
                a.m_date=appointment.attribute("Date");
                a.m_startTime=appointment.attribute("StartTime");
                a.m_endTime=appointment.attribute("EndTime");
                a.m_isFullDay=appointment.attribute("IsAllDay").toInt();
                a.m_category=appointment.attribute("Category");

                if (dbm.isOpen())
                {

                    bool success = dbm.addAppointment(a);

                    if(success)
                    {
                       //qDebug()<<"appointment added to database";
                    }
                }
            }
        }

        file.close();
        //qDebug() << "Import Finished";
        LoadDatabaseAppointmentsToAppointmentList();
        UpdateCalendar();
        ShowDayAppointmentsTableView();

}



void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::gotoNextMonth()
{
    selectedMonth =selectedDate.month()+1;
       selectedYear =selectedDate.year();
       selectedDay =selectedDate.day();
       //qDebug()<<"year = "<<selectedYear;
       //qDebug()<<"month = "<<selectedMonth;
       //qDebug()<<"day = "<<selectedDay;

       if (selectedMonth >= 13) {
           selectedMonth = 1;
           selectedYear =selectedYear+1;
           UpdateHolidaysInAppointmentList(selectedYear);
           UpdateBirthdaysInAppointmentList(selectedYear);
           UpdateCalendar();
           ShowDayAppointmentsTableView();
       }

       //qDebug()<<"year = "<<selectedYear;
       //qDebug()<<"month = "<<selectedMonth;
       //qDebug()<<"day = "<<selectedDay;

       selectedDate.setDate(selectedYear,selectedMonth,selectedDay);

       //qDebug()<<selectedDate.toString();


       QString monthName=monthNames[selectedMonth-1];
       QString monthYear=monthName+" "+QString::number(selectedYear);
       ui->labelMonthYear->setText(monthYear);
       ui->labelSelectedDate->setText(selectedDate.toString());
       //RemoveAllHolidayAppointments(); //calculated using selected date
       //AddHolidayAppointments();
       UpdateCalendar();
       ShowDayAppointmentsTableView();

}

void MainWindow::gotoPreviousMonth()
{
    selectedMonth =selectedDate.month()-1;
        selectedYear =selectedDate.year();
        selectedDay =selectedDate.day();
        //qDebug()<<"year = "<<selectedYear;
        //qDebug()<<"month = "<<selectedMonth;
        //qDebug()<<"day = "<<selectedDay;

        if (selectedMonth < 1)
        {
            selectedMonth = 12;
            selectedYear=selectedYear-1;

            UpdateHolidaysInAppointmentList(selectedYear);
            UpdateBirthdaysInAppointmentList(selectedYear);
            UpdateCalendar();
            ShowDayAppointmentsTableView();
        }
        selectedDate.setDate(selectedYear,selectedMonth,selectedDay);

        //qDebug()<<selectedDate.toString();
        UpdateCalendar();

        QString monthName=monthNames[selectedMonth-1];
        QString monthYear=monthName+" "+QString::number(selectedYear);
        ui->labelMonthYear->setText(monthYear);
        ui->labelSelectedDate->setText(selectedDate.toString());
        ShowDayAppointmentsTableView();
}

void MainWindow::UpdateCalendar()
{
    int cellIndex=0;
    ui->tableWidget->clearContents();

    //Initailise with empty cells
    for (int row=0; row<ui->tableWidget->rowCount(); ++row)
    {
        ui->tableWidget->setRowHeight(row,100);
        for(int col=0; col<ui->tableWidget->columnCount(); ++col)
        {
            ui->tableWidget->setItem(row, col,
                                     new QTableWidgetItem(""));
            cellIndex = (7 * row) + col;
            dayArray[cellIndex]=0;
        }
    }

    QStringList days;
    for (int weekDay = 1; weekDay <= 7; ++weekDay) {
        QString day =QLocale::system().dayName(weekDay); //boldFormat
        //qDebug()<<day;
        days.append(day);
    }
    ui->tableWidget->setHorizontalHeaderLabels(days);
    ui->tableWidget->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);

    QDate date(selectedDate.year(), selectedDate.month(), 1);
    //qDebug()<<"start date = "<<date.toString();

    int row=0;
    int dayValue=0;
    while (date.month() == selectedDate.month()) {
        int weekDay = date.dayOfWeek();
        //qDebug()<<"weekDay ="<<weekDay;

        dayValue= date.day();
        // qDebug()<<"DayValue = "<<dayValue;
        cellIndex = (7 * row) + weekDay-1;
        //qDebug()<<"cellIndex = "<<cellIndex;

        dayArray[cellIndex]=dayValue;

        dayItem = new QTableWidgetItem(QString::number(dayValue));

        if (date==QDate::currentDate())
        {
            //dayItem->setTextColor(Qt::red);
            dayItem->setBackgroundColor(Qt::yellow);

        }

        ui->tableWidget->setItem(row, weekDay-1,dayItem);

        //-------------------------------------

        date = date.addDays(1);
        if (weekDay == 7 && date.month() == selectedDate.month())
        {
            // qDebug()<<"Insert row here?..";
            row=row+1;
        }
    }

    //Now add appointments
    int dayVal=0;
    QDate loopDate;   

    for (int row=0; row<ui->tableWidget->rowCount(); ++row)
    {
        for(int col=0; col<ui->tableWidget->columnCount(); ++col)
        {
            dayVal =dayArray[(7 * row) + col];
            loopDate =QDate(selectedDate.year(),selectedDate.month(),dayVal);
            //qDebug()<<"updateCalendar: Add Appointment currentDate = "<<loopDate.toString();

            foreach(Appointment a, appointmentList)
            {
                QDate d =QDate::fromString(a.m_date);
                if(loopDate ==d)
                {
                    if(a.m_category=="Event")
                    {
                       //Get current contents and append
                        QString cellVal=ui->tableWidget->item(row,col)->text();

                        appointmentItem= new QTableWidgetItem(cellVal+"\n"+ a.m_title);
                        appointmentItem->setTextColor(Qt::blue);                         

                        if(a.m_isFullDay==1)
                        {
                            appointmentItem->setBackgroundColor(Qt::lightGray);
                        }

                        //Need to check for currentDate and set background color
                        if(loopDate==QDate::currentDate())
                        {
                            appointmentItem->setBackgroundColor(Qt::yellow);
                        }
                        ui->tableWidget->setItem(row, col,appointmentItem);

                    }
                    else if((a.m_category=="Birthday") && (a.m_showBirthday==1))
                    {
//
                        QString cellVal=ui->tableWidget->item(row,col)->text();                        
                        appointmentItem= new QTableWidgetItem(cellVal+"\n"+ a.m_title);
                        appointmentItem->setTextColor(Qt::blue);
                        //appointmentItem->setTextColor(Qt::darkCyan);

                        if(a.m_isFullDay==1)
                        {
                            appointmentItem->setBackgroundColor(Qt::lightGray);
                        }
                        //Need to check for currentDate and set background color
                        if(loopDate==QDate::currentDate())
                        {
                            appointmentItem->setBackgroundColor(Qt::yellow);
                        }
                        ui->tableWidget->setItem(row, col,appointmentItem);

                    }
                    else if(a.m_category=="Family"){
                        QString cellVal=ui->tableWidget->item(row,col)->text();
                        //qDebug()<<"cellVal = "<<cellVal;
                        appointmentItem= new QTableWidgetItem(cellVal+"\n"+ a.m_title);
                        appointmentItem->setTextColor(Qt::blue);

                        if(a.m_isFullDay==1)
                        {
                            appointmentItem->setBackgroundColor(Qt::lightGray);
                        }
                        //Need to check for currentDate and set background color
                        if(loopDate==QDate::currentDate())
                        {
                            appointmentItem->setBackgroundColor(Qt::yellow);
                        }
                        ui->tableWidget->setItem(row, col,appointmentItem);

                    }
                    else if((a.m_category=="Leisure")||
                            (a.m_category=="Meeting")||
                            (a.m_category=="Business")||
                            (a.m_category=="Vacation"))

                    {
                        QString cellVal=ui->tableWidget->item(row,col)->text();

                        appointmentItem= new QTableWidgetItem(cellVal+"\n"+ a.m_title);
                        appointmentItem->setTextColor(Qt::blue);

                        if(a.m_isFullDay==1)
                        {
                            appointmentItem->setBackgroundColor(Qt::lightGray);
                        }
                        //Need to check for currentDate and set background color
                        if(loopDate==QDate::currentDate())
                        {
                            appointmentItem->setBackgroundColor(Qt::yellow);
                        }
                        ui->tableWidget->setItem(row, col,appointmentItem);

                    }

                    else if(a.m_category=="Holiday"){
                        QString cellVal=ui->tableWidget->item(row,col)->text();

                        appointmentItem= new QTableWidgetItem(cellVal+"\n"+ a.m_title);
                        appointmentItem->setTextColor(Qt::black);

                        if(a.m_isFullDay==1)
                        {

                            appointmentItem->setBackgroundColor(Qt::darkYellow);
                        }
                        //Need to check for currentDate and set background color
                        if(loopDate==QDate::currentDate())
                        {
                            appointmentItem->setBackgroundColor(Qt::yellow);
                        }
                        ui->tableWidget->setItem(row, col,appointmentItem);

                    }
                    else if(a.m_category=="Medical"){
                        QString cellVal=ui->tableWidget->item(row,col)->text();
                        //qDebug()<<"cellVal = "<<cellVal;
                        appointmentItem= new QTableWidgetItem(cellVal+"\n"+ a.m_title);

                        appointmentItem->setTextColor(Qt::blue);
                        //Need to check for currentDate and set background color
                        if(a.m_isFullDay==1)
                        {
                            appointmentItem->setBackgroundColor(Qt::lightGray);
                            appointmentItem->setTextColor(Qt::red);
                        }
                        if(loopDate==QDate::currentDate())
                        {
                            appointmentItem->setBackgroundColor(Qt::yellow);
                        }
                        ui->tableWidget->setItem(row, col,appointmentItem);
                    }

                }
            }
        }
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{

    int day =dayArray[(7 * row) + column];    
    QDate theDate =QDate(selectedDate.year(),selectedDate.month(),day);
    //qDebug()<<"Selected Date = "<<theDate.toString();
    selectedDate=theDate;
    ui->labelSelectedDate->setText(theDate.toString());
    ShowDayAppointmentsTableView();

}

void MainWindow::on_pushButtonNextMonth_clicked()
{
    gotoNextMonth();
}

void MainWindow::on_pushButtonPreviousMonth_clicked()
{
    gotoPreviousMonth();
}

void MainWindow::on_actionNew_Appointment_triggered()
{
    NewAppointment();
}

void MainWindow::on_actionCheck_For_Reminders_triggered()
{
    DisplayRemindersForDate(selectedDate);
}


void MainWindow::on_actionNew_Contact_triggered()
{
    NewContact();
}



void MainWindow::on_tableViewContacts_doubleClicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data();
    selectedContactDbId =idd.toInt();
    //qDebug()<<"SelectedContactId = "<<selectedContactDbId;
    UpdateContact(selectedContactDbId);
}


void MainWindow::RemoveAllAppointments()
{

    dbm.deleteAllAppointments();
    dbm.removeAllReminders();

    UpdateAppointmentList();
    UpdateCalendar();
    ShowDayAppointmentsTableView();
}


void MainWindow::on_pushButtonShowQuickFullDetails_clicked()
{
    ui->tableViewContacts->hideColumn(0); //index
        //ui->tableViewContacts->hideColumn(13); //BithID

        if (flagQuickView)
        {
            ui->tableViewContacts->hideColumn(2); //mid names
            ui->tableViewContacts->hideColumn(5); //street
            ui->tableViewContacts->hideColumn(6); //district
            ui->tableViewContacts->hideColumn(8); //county
            ui->tableViewContacts->hideColumn(9); //postcode
            ui->tableViewContacts->hideColumn(10); //country
            ui->tableViewContacts->hideColumn(11); //telephone
            flagQuickView=false;
        }
        else {

            ui->tableViewContacts->showColumn(2); //mid names
            ui->tableViewContacts->showColumn(5); //street
            ui->tableViewContacts->showColumn(6); //district
            ui->tableViewContacts->showColumn(7); //city
            ui->tableViewContacts->showColumn(8); //county
            ui->tableViewContacts->showColumn(9); //postcode
            ui->tableViewContacts->showColumn(10); //country
            ui->tableViewContacts->showColumn(11); //telephone
            flagQuickView=true;
        }
}


void MainWindow::on_pushButtonMailTo_clicked()
{
    QString url="mailto:?to="+selectedContact.m_email+"&subject=Enter the subject&body=Enter message";
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}



void MainWindow::on_actionCheck_For_Upcoming_Birthdays_triggered()
{
    checkForBirthdaysNextSevenDays();
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout *aboutDialog = new DialogAbout(this);
    aboutDialog->setModal(false);
    aboutDialog->exec();
}

void MainWindow::on_actionDelete_All_Contacts_triggered()
{
    //clearAllBirthdaysOnCalendar();
    dbm.removeAllContacts();
    contactList.clear();
    contactModel->clearAllContacts();    
    UpdateAppointmentList(); //no birthdays now
    UpdateCalendar();
    DisplayContactsOnTableView(); //should be nothing

}

void MainWindow::on_actionExport_Contacts_XML_triggered()
{
    exportContactsXML();
}

void MainWindow::on_actionExport_Appointments_XML_triggered()
{
    exportAppointmentsXML();
}

void MainWindow::on_actionImport_Contacts_XML_triggered()
{
    importContactsXML();
}

void MainWindow::on_actionImport_Appointments_XML_triggered()
{
    importAppointmentsXML();
}

void MainWindow::on_actionCheck_For_Reminders_Today_triggered()
{
    QDate currentDate =QDate::currentDate();
    DisplayRemindersForDate(currentDate);
}

void MainWindow::on_actionSystem_Notifications_triggered()
{
    if (ui->actionSystem_Notifications->isChecked())
    {
        ui->actionSystem_Notifications->setChecked(true);
        this->flagNotifications=true;
    }
    else {
        ui->actionSystem_Notifications->setChecked(false);
        this->flagNotifications=false;
    }
}


void MainWindow::on_tableViewDays_doubleClicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data();
    int selectedDbId =idd.toInt();
    //qDebug()<<"SelectedId = "<<selectedDbId;

    if(selectedDbId<=0)
    {
        //qDebug()<<"Special on-the-fly event";
        return;
    }
    else {
        UpdateAppointment(selectedDbId);
    }
}

void MainWindow::on_actionDelete_All_Appointments_triggered()
{
    RemoveAllAppointments();
}

void MainWindow::on_tableViewContacts_clicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data();
    selectedContactDbId =idd.toInt();
    //qDebug()<<"SelectedContactId = "<<selectedCont
    selectedContact=dbm.getContactByID(selectedContactDbId);

}


void MainWindow::on_actionCheck_Reminders_Hourly_triggered()
{
    if(ui->actionCheck_Reminders_Hourly->isChecked())
    {
        ui->actionCheck_Reminders_Hourly->setChecked(true);
        flagRemindersHourly=true;
    }
    else {
        ui->actionCheck_Reminders_Hourly->setChecked(false);
        flagRemindersHourly=false;
    }

}

void MainWindow::timerUpdateSlot()
{
    QTime currentTime =QTime::currentTime();
    statusTimeLabel->setText(currentTime.toString("hh:mm:ss"));
    //qDebug()<<"Timer Tick Tock...";
    //qDebug()<<"flagreminderHourly = "<<flagRemindersHourly;
    if (flagRemindersHourly==true)
    {
        //qDebug()<<"Checking reminders hourly";
        CheckForRemindersOnHour();
    }

}

void MainWindow::sendReminderMessage()
{
    ui->statusBar->showMessage("New reminder notification .. ",10000); //10 seconds
    QString h="Reminder Notification";
    QString m=reminderSingleShot.m_details;
    MessageData *msg = new MessageData(h,m);
    DbusSessionMessage *dbus = new DbusSessionMessage(this);
    dbus->displayMessage(*msg);
}
