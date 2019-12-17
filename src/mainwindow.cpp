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

    ui->tableViewContacts->horizontalHeader()->setVisible(true);
    ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewContacts->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewContacts->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewContacts->verticalHeader()->setVisible(false);

    contactModel = new ContactModel; //stores all contacts
    proxyModelContacts= new ProxyModelContacts;
    LoadContactsListFromDatabase(); //loads contactlist
    DisplayContactsOnTableView();

    //Birthdays now  appointments
    RemoveAllBirthdayAppointmentsFromDatebase();
    AddBirthdayAppointmentsToDatabase(selectedDate.year()); //uses contactlist
    //Holidays special appointments
    RemoveAllHolidayAppointmentsFromDatabase();
    AddHolidayAppointmentsToDatabase(selectedDate.year());

    LoadAppointmentsListFromDatabase();
    LoadReminderListFromDatabase();
    updateCalendar();
    ShowDayAppointments();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::newAppointment()
{
    DialogAppointment *appointmentDialog = new  DialogAppointment(this,&selectedDate);
    appointmentDialog->setModal(true);

    if (appointmentDialog->exec() == QDialog::Accepted ) {

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
            //qDebug()<<"Add Appointment: Appointment ID = "<<appointmentId;
        }


        if(reminderRequested==1)
        {

            //Reminder requested
            QString dateStr="("+QString::number(selectedDate.day())+"/"
                    +QString::number(selectedDate.month())+"/"
                    +QString::number(selectedDate.year())+")";

            QString reminderDetails=title
                    +" ("+location
                    + ") "+dateStr;

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
            r.m_appointmentId=appointmentId;
            r.m_reminderDate=selectedDate.addDays(-reminderDays).toString();
            r.m_reminderTime=appointmentStartTime.toString();;

            if (dbm.isOpen())
            {
                bool success =dbm.addReminder(r);
                if (success){
                    //qDebug()<<"Reminder added with appointmentId = "<<appointmentId;
                }
                else {
                   // qDebug()<<"Reminder add failed!";
                }
            }
        }

    }

    LoadAppointmentsListFromDatabase();
    LoadReminderListFromDatabase();
    updateCalendar();
    ShowDayAppointments();
}

void MainWindow::LoadAppointmentsListFromDatabase()
{
    //initialisation - do at startup
    appointmentList.clear();
    QList<Appointment> tmpList = dbm.getAllAppointments();
    foreach(Appointment a, tmpList)
    {
        appointmentList.append(a);
    }
}

void MainWindow::LoadReminderListFromDatabase()
{
    reminderList.clear();
    QList<Reminder> tmpList = dbm.getAllReminders();
    foreach(Reminder r, tmpList)
    {
        reminderList.append(r);
    }
}

void MainWindow::ShowDayAppointments()
{
    QList<Appointment> dayList =QList<Appointment>();
    //SortAppointmentListByStartTime();
    foreach(Appointment a, appointmentList)
    {
        QDate adate = QDate::fromString(a.m_date);

        if(adate==selectedDate)
        {
            dayList.append(a);
        }
    }

    std::sort(dayList.begin(), dayList.end(), compare);
    dayListModel = new DayListModel(dayList);
    ui->listViewDay->setModel(dayListModel);
}

void MainWindow::LoadContactsListFromDatabase()
{
    //initialisation - do at startup
    contactsList.clear();
    QList<Contact> tmpList = dbm.getAllContacts();
    foreach(Contact c, tmpList)
    {
        contactsList.append(c);
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
        //this->addToCalendar=contactDialog->getAddToCalendar();
        //this->birthDateAppointmentId=contactDialog->getBirthDateId();

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
        c.m_addbirthday=addBirthdayToCalendar;
        c.m_birthdayAppointmentId=0;

        //Create a birthday appointment
        Appointment ba;
        ba.m_title="Birthday";
        ba.m_location=c.m_city;
        ba.m_description=c.m_firstname+" "+c.m_lastname;
        QDate currentDate =QDate::currentDate();
        QDate borndate =QDate::fromString(c.m_birthdate);
        QDate currentBDay =QDate(currentDate.year(), borndate.month(), borndate.day());
        ba.m_date=currentBDay.toString();
        ba.m_category="Birthday";
        ba.m_isFullDay=1;
        ba.m_addBirthday=addBirthdayToCalendar;

        if (dbm.isOpen())
        {
            int appointmentId = dbm.addAppointment(ba);

            c.m_birthdayAppointmentId=appointmentId; //foreign link key (delete contact delete appointment)
            dbm.addContact(c);

//            if(success) //check contact added to database
//            {
//                //qDebug()<<"contact added to database";
//            }
        }
    }
    LoadContactsListFromDatabase(); //refresh contactslist here
    DisplayContactsOnTableView(); //display

    RemoveAllBirthdayAppointmentsFromDatebase();
    AddBirthdayAppointmentsToDatabase(selectedDate.year()); //uses refreshed contactslist

    LoadAppointmentsListFromDatabase();
    updateCalendar();
    ShowDayAppointments();

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
               int appointmentlink = currentContact.m_birthdayAppointmentId;
               dbm.removeContactById(dbID);              
               dbm.deleteAppointmentById(appointmentlink);

               RemoveAllBirthdayAppointmentsFromDatebase();


               LoadContactsListFromDatabase(); //clears and reloads contact lsit
               AddBirthdayAppointmentsToDatabase(selectedDate.year());

               DisplayContactsOnTableView();

               LoadAppointmentsListFromDatabase();
               updateCalendar();
               ShowDayAppointments();

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
           c.m_addbirthday=addBirthdayToCalendar;


           if (dbm.isOpen())
           {
                //Update Birthday Appointment

               Appointment ba =dbm.getAppointmentByID(c.m_birthdayAppointmentId);
               ba.m_title="Birthday";
               ba.m_location=c.m_city;
               ba.m_description=c.m_firstname+" "+c.m_lastname;
               QDate currentDate =QDate::currentDate();
               QDate borndate =QDate::fromString(c.m_birthdate);
               QDate currentBDay =QDate(currentDate.year(), borndate.month(), borndate.day());
               ba.m_date=currentBDay.toString();
               ba.m_category="Birthday";
               ba.m_isFullDay=1;
               ba.m_addBirthday=addBirthdayToCalendar;

               dbm.updateAppointment(ba,c.m_birthdayAppointmentId);

               if(dbm.updateContact(c,dbID)) //add contact to database
               {
                   //qDebug()<<"contact update added to database";
               }

           }
       }

       RemoveAllBirthdayAppointmentsFromDatebase();
       LoadContactsListFromDatabase(); //clears and reloads contactList
       DisplayContactsOnTableView();       
       AddBirthdayAppointmentsToDatabase(selectedDate.year());
       LoadAppointmentsListFromDatabase();
       updateCalendar();
       ShowDayAppointments();
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
            if(currentAppointment.m_hasReminder==1)
            {
                //int reminderKey = currentAppointment.m_reminderId;
                dbm.removeReminderById(currentAppointment.m_id);
            }

            dbm.deleteAppointmentById(dbID);

            LoadAppointmentsListFromDatabase();
            LoadReminderListFromDatabase();
            updateCalendar();
            ShowDayAppointments();
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
        a.m_addBirthday=0;

        if (dbm.isOpen())
        {
            dbm.updateAppointment(a,dbID);
            //qDebug()<<"Apointment update: success ="<<success;
        }

        if(reminderRequested==1)
        {

            //Reminder requested
            QString dateStr="("+QString::number(selectedDate.day())+"/"
                    +QString::number(selectedDate.month())+"/"
                    +QString::number(selectedDate.year())+")";

            QString reminderDetails=title
                    +" ("+location
                    + ") "+dateStr;

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
                //qDebug()<<"Reminder update success = "<<success;
            }
        }
        LoadAppointmentsListFromDatabase();
        LoadReminderListFromDatabase();
        ShowDayAppointments();
    }
}

bool MainWindow::compare(const Appointment &first, const Appointment &second)
{
    QTime firstStarts =QTime::fromString(first.m_startTime);
    QTime secondStarts =QTime::fromString(second.m_startTime);

    int fx=60*60*firstStarts.hour()+60*firstStarts.minute(); //seconds
    int sx=60*60*secondStarts.hour()+60*secondStarts.minute();

    if (fx < sx)
        {
            return true;
        }
        else if (fx > sx)
        {
            return false;
        }
    return false;
}

void MainWindow::checkForRemindersMidday()
{
    QDate currentDate=QDate::currentDate();
    QTime currentTime= QTime::currentTime();
    int currentHour =currentTime.hour();
    int currentMinute = currentTime.minute();
    int currentSecond =currentTime.second();

    if(currentHour==12 && currentMinute==0 &&currentSecond==0 ) //midday
    {
        //checkForBirthdaysNextSevenDays();

        foreach(Reminder r, reminderList){

               QDate reminderDate =QDate::fromString(r.m_reminderDate);

               if (reminderDate==currentDate)
               {
                   QString str=r.m_details;
                   QMessageBox::information(this,"Reminder",str);
               }
               //fire single shot timer
               if (notificationsFlag){
                   reminderSingleShot=r;
                   QTimer::singleShot(2000, this, SLOT(sendReminderMessage()));
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
        if (notificationsFlag){
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

     if (notificationsFlag){

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
        foreach(Contact c, contactsList)  //use contacts!!
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


void MainWindow::RemoveAllHolidayAppointmentsFromDatabase()
{
    QList<Appointment> appointmentList =dbm.getAllAppointments();

    foreach (Appointment a, appointmentList)
    {
        int id =a.m_id;

        if(a.m_category=="Holiday")
        {
            //qDebug()<<"Delete holiday appointment id = "<<id;
            dbm.deleteAppointmentById(id);
        }
    }
}

void MainWindow::AddHolidayAppointmentsToDatabase(int year)
{
    //Create holidays on-the-fly to save resources

    holidayList.clear();

    Holiday h1;
    h1.m_id=1;
    h1.m_name="Christmas ";
    h1.m_date=QDate(year,12,25).toString();
    holidayList.append(h1);

    Holiday h2;
    h2.m_id=2;
    h2.m_name="Boxing Day ";
    h2.m_date=QDate(year,12,26).toString();
    holidayList.append(h2);

    //Calculate Easter and add
    QDate easterSunday =CalculateEaster(year);
    //qDebug()<<"Easter Sunday 2019 = "<<easterSunday.toString();
    Holiday h3;
    h3.m_id=3;
    h3.m_name="Easter ";
    h3.m_date=easterSunday.toString();
    holidayList.append(h3);

    Holiday h4;
    h4.m_id=4;
    h4.m_name="Good Friday ";
    h4.m_date=easterSunday.addDays(-2).toString();
    holidayList.append(h4);

    Holiday h5;
    h5.m_id=5;
    h5.m_name="Easter Monday ";
    h5.m_date=easterSunday.addDays(1).toString();
    holidayList.append(h5);

    Appointment ha;

    foreach(Holiday h, holidayList)
    {

        ha.m_title=h.m_name;
        ha.m_description= "Holiday ";
        ha.m_location="Home";
        ha.m_date=h.m_date;
        ha.m_isFullDay=1;
        ha.m_category="Holiday";
        if (dbm.isOpen())
        {
            appointmentId=dbm.addAppointment(ha);
            //qDebug()<<"Add Appointment: Appointment ID = "<<appointmentId;
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
               c.m_addbirthday=0;
               c.m_birthdayAppointmentId=0;


               //Create a birthday appointment
               Appointment ba;
               ba.m_title="Birthday";
               ba.m_location=c.m_city;
               ba.m_description=c.m_firstname+" "+c.m_lastname;
               QDate currentDate =QDate::currentDate();
               QDate borndate =QDate::fromString(c.m_birthdate);
               QDate currentBDay =QDate(currentDate.year(), borndate.month(), borndate.day());
               ba.m_date=currentBDay.toString();
               ba.m_category="Birthday";
               ba.m_isFullDay=1;
               ba.m_addBirthday=0;

               if (dbm.isOpen())
               {
                   int appointmentId = dbm.addAppointment(ba);

                   c.m_birthdayAppointmentId=appointmentId; //foreign link key (delete contact delete appointment)
                   dbm.addContact(c);
               }


//               if (dbm.isOpen())
//               {
//                   bool success = dbm.addContact(c);
//                   if(success) //add contact to database
//                   {
//                       //qDebug()<<"contact added to database";
//                       contactsList.append(c);
//                   }
//               }
           }
       }
       file.close();
       //qDebug() << "Finished";

       LoadContactsListFromDatabase(); //refresh contactslist here
       DisplayContactsOnTableView(); //display

       RemoveAllBirthdayAppointmentsFromDatebase(); //?just in case
       AddBirthdayAppointmentsToDatabase(selectedDate.year());  //alos loads contacts again
       updateCalendar();

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
        LoadAppointmentsListFromDatabase();
        updateCalendar();
        ShowDayAppointments();
}

void MainWindow::timerUpdateSlot()
{
    QTime currentTime =QTime::currentTime();
    statusTimeLabel->setText(currentTime.toString("hh:mm:ss"));
    checkForRemindersMidday();
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
           //qDebug()<<"Selected Year = "<<selectedYear;
           RemoveAllHolidayAppointmentsFromDatabase();
           AddHolidayAppointmentsToDatabase(selectedYear);
           RemoveAllBirthdayAppointmentsFromDatebase();
           AddBirthdayAppointmentsToDatabase(selectedYear);
           LoadAppointmentsListFromDatabase();
           updateCalendar();
           ShowDayAppointments();

       }

       //qDebug()<<"year = "<<selectedYear;
       //qDebug()<<"month = "<<selectedMonth;
       //qDebug()<<"day = "<<selectedDay;

       selectedDate.setDate(selectedYear,selectedMonth,selectedDay);

       //qDebug()<<selectedDate.toString();
       updateCalendar();

       QString monthName=monthNames[selectedMonth-1];
       QString monthYear=monthName+" "+QString::number(selectedYear);
       ui->labelMonthYear->setText(monthYear);
       ui->labelSelectedDate->setText(selectedDate.toString());
       //RemoveAllHolidayAppointments(); //calculated using selected date
       //AddHolidayAppointments();
        ShowDayAppointments();
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
            //qDebug()<<"Selected Year = "<<selectedYear;
            RemoveAllHolidayAppointmentsFromDatabase();
            AddHolidayAppointmentsToDatabase(selectedYear);
            RemoveAllBirthdayAppointmentsFromDatebase();
            AddBirthdayAppointmentsToDatabase(selectedYear);
            LoadAppointmentsListFromDatabase();
            updateCalendar();
            ShowDayAppointments();

        }
        selectedDate.setDate(selectedYear,selectedMonth,selectedDay);

        //qDebug()<<selectedDate.toString();
        updateCalendar();

        QString monthName=monthNames[selectedMonth-1];
        QString monthYear=monthName+" "+QString::number(selectedYear);
        ui->labelMonthYear->setText(monthYear);
        ui->labelSelectedDate->setText(selectedDate.toString());
        ShowDayAppointments();
        //ui->labelSelectedDate->setText(selectedDate.toString());
        //RemoveAllHolidayAppointments();
        //AddHolidayAppointments();
}

void MainWindow::updateCalendar()
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
    //QFont testFont =QFont( "lucida", 12, QFont::Bold);

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
                    else if((a.m_category=="Birthday") && (a.m_addBirthday==1))
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
    ShowDayAppointments();
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
    newAppointment();
}

void MainWindow::on_actionCheck_For_Reminders_triggered()
{
    DisplayRemindersForDate(selectedDate);
}

void MainWindow::on_listViewDay_doubleClicked(const QModelIndex &index)
{
   int selectedRowIdx=index.row();
   Appointment tmp =dayListModel->getAppointment(selectedRowIdx);
   int dbId =tmp.m_id;
   UpdateAppointment(dbId);
}

void MainWindow::on_actionNew_Contact_triggered()
{
    NewContact();
}

void MainWindow::on_tableViewContacts_doubleClicked(const QModelIndex &index)
{
    int selectedRowIdx=index.row();
    Contact tmp =contactModel->getContact(selectedRowIdx);
    int dbId =tmp.m_id;
    UpdateContact(dbId);    
}


void MainWindow::RemoveAllAppointments()
{
    dayListModel->clearAllAppointment();
    dbm.deleteAllAppointments();
    dbm.removeAllReminders();

    RemoveAllHolidayAppointmentsFromDatabase();
    AddHolidayAppointmentsToDatabase(selectedDate.year());

    RemoveAllBirthdayAppointmentsFromDatebase();
    AddBirthdayAppointmentsToDatabase(selectedDate.year());

    LoadAppointmentsListFromDatabase();
    updateCalendar();
    ShowDayAppointments();

}

void MainWindow::RemoveAllBirthdayAppointmentsFromDatebase()
{
    QList<Appointment> appointmentList =dbm.getAllAppointments();

    foreach (Appointment a, appointmentList)
    {
        int id =a.m_id;

        if(a.m_category=="Birthday")
        {
            dbm.deleteAppointmentById(id);
        }
    }
}

void MainWindow::on_actionClear_All_Appointments_triggered()
{
    RemoveAllAppointments();
}

void MainWindow::AddBirthdayAppointmentsToDatabase(int year)
{

    //Create a birthday appointments and add
    //LoadContactsListFromDatabase();
    //Assumes contactlist uptodate
    Appointment ba;

    foreach(Contact c, contactsList)
    {
        ba.m_title="Birthday ";
        ba.m_location=c.m_city;
        ba.m_description=c.m_firstname+" "+c.m_lastname;
        QDate borndate =QDate::fromString(c.m_birthdate);
        QDate currentBDay =QDate(year, borndate.month(), borndate.day());
        ba.m_date=currentBDay.toString();
        ba.m_isFullDay=1;
        ba.m_category="Birthday";
        ba.m_addBirthday=c.m_addbirthday;
        if (dbm.isOpen())
        {
            appointmentId=dbm.addAppointment(ba);
            //qDebug()<<"Add Appointment: Appointment ID = "<<appointmentId;
        }
    }
}

void MainWindow::on_action_Add_Current_Birthdays_triggered()
{
    //Add birthdays
    AddBirthdayAppointmentsToDatabase(selectedDate.year());
}

void MainWindow::on_actionRemove_All_Birthday_triggered()
{
    RemoveAllBirthdayAppointmentsFromDatebase();
}

void MainWindow::on_pushButtonShowQuickFullDetails_clicked()
{
    ui->tableViewContacts->hideColumn(0); //index
        ui->tableViewContacts->hideColumn(13); //BithID

        if (quickViewFlag)
        {
            ui->tableViewContacts->hideColumn(2); //mid names
            ui->tableViewContacts->hideColumn(5); //street
            ui->tableViewContacts->hideColumn(6); //district
            ui->tableViewContacts->hideColumn(8); //county
            ui->tableViewContacts->hideColumn(9); //postcode
            ui->tableViewContacts->hideColumn(10); //country
            ui->tableViewContacts->hideColumn(11); //telephone
            quickViewFlag=false;
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
            quickViewFlag=true;
        }
}

void MainWindow::on_pushButtonSort_clicked()
{
    Qt::SortOrder order=Qt::AscendingOrder;
        contactModel->clearAllContacts();
        foreach(Contact c, contactsList)
        {
            contactModel->AddContact(c);
        }

        if(sortOrderFlag==false){

            order=Qt::DescendingOrder;
            sortOrderFlag=true;
        }
        else if (sortOrderFlag==true){

            order=Qt::AscendingOrder;
            sortOrderFlag=false;
        }

        proxyModelContacts->setSourceModel(contactModel);
        ui->tableViewContacts->setModel(proxyModelContacts);
        ui->tableViewContacts->setSortingEnabled(true);
        ui->tableViewContacts->sortByColumn(2,order);
        //ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

void MainWindow::on_pushButtonMailTo_clicked()
{
    QString url="mailto:?to="+selectedContact.m_email+"&subject=Enter the subject&body=Enter message";
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

void MainWindow::on_tableViewContacts_clicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data();
    selectedContactDbId =idd.toInt();
    selectedContact=dbm.getContactByID(selectedContactDbId);
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
    contactsList.clear();
    contactModel->clearAllContacts();
    DisplayContactsOnTableView(); //should be nothing
    RemoveAllBirthdayAppointmentsFromDatebase();
    LoadAppointmentsListFromDatabase();
    updateCalendar();

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
        this->notificationsFlag=true;
    }
    else {
        ui->actionSystem_Notifications->setChecked(false);
        this->notificationsFlag=false;
    }
}


