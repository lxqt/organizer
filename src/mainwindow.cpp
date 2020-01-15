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
    setWindowTitle(tr("Organizer"));
    //set layout first
    QMainWindow::centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    //Setup database
    dbm.openDatabase();
    dbm.createDatebaseTables();

    selectedDate = QDate::currentDate();

    dateInfoLabel =new QLabel(this);
    dateInfoLabel->setText(tr("Selected Date: "));
    selectedDateLabel = new QLabel(this);
    QFont font1 = selectedDateLabel->font();
    font1.setWeight(QFont::Bold);
    selectedDateLabel->setFont(font1);

    selectedDateLabel->setText(selectedDate.toString());
    ui->statusBar->addPermanentWidget(dateInfoLabel);
    ui->statusBar->addPermanentWidget(selectedDateLabel);

    selectedMonth=selectedDate.month();
    selectedYear=selectedDate.year();

    QFont font = ui->labelMonthYear->font();
    font.setWeight(QFont::Bold);
    ui->labelMonthYear->setFont(font);
    QString monthName=monthNames[selectedMonth-1];
    QString monthYear=monthName+" "+QString::number(selectedYear);
    ui->labelMonthYear->setText(monthYear);

    ui->tableViewContacts->horizontalHeader()->setVisible(true);
    ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewContacts->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewContacts->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewContacts->verticalHeader()->setVisible(false);


    //Code refactoring - translations

    //File menu
    ui->menuFile->setTitle(tr("&File"));
    //ui->menuFile->setToolTip(tr("File"));
    ui->actionExport_Contacts_XML->setText(tr("Export Contacts XML"));
    ui->actionExport_Contacts_XML->setToolTip(tr("Export Contacts XML"));
    ui->actionImport_Contacts_XML->setText(tr("Import Contacts XML"));
    ui->actionImport_Contacts_XML->setToolTip(tr("Import Contacts XML"));
    ui->actionExit->setText(tr("Exit"));
    ui->actionExit->setToolTip(tr("Exit"));

    //Edit menu
    ui->menuEdit->setTitle(tr("&Edit"));
    //ui->menuEdit->setToolTip(tr("Edit"));
    ui->actionDelete_All_Appointments->setText(tr("Delete All Appointments"));
    ui->actionDelete_All_Appointments->setToolTip(tr("Delete All Appointments"));
    ui->actionDelete_All_Contacts->setText(tr("Delete All Contacts"));
    ui->actionDelete_All_Contacts->setToolTip(tr("Delete All Contacts"));

    //Action menu
    ui->menuAction->setTitle(tr("&Action"));
    ui->menuAction->setToolTip(tr("Action"));

    ui->actionNew_Appointment->setText(tr("New Appointment"));
    ui->actionNew_Appointment->setToolTip(tr("New Appointment"));
    ui->actionGenerate_Repeat_Appointments->setText(tr("Generate Repeat Appointments"));
    ui->actionGenerate_Repeat_Appointments->setToolTip(tr("Generate Repeat Appointments"));
    ui->actionNew_Contact->setText(tr("New Contact"));
    ui->actionNew_Contact->setToolTip(tr("New Contact"));
    ui->actionUpcoming_Schedule->setText(tr("Upcoming Schedule"));
    ui->actionUpcoming_Schedule->setToolTip(tr("Upcoming Schedule"));
    ui->actionCheck_For_Birthdays->setText(tr("Check For Birthdays"));
    ui->actionCheck_For_Birthdays->setToolTip(tr("Check For Birthdays"));

    //Navigation menu
    ui->menuNaviagation->setTitle(tr("&Navigation"));
    //ui->menuNaviagation->setToolTip(tr("Navigation"));

    ui->actionNext_Day->setText(tr("Next Day"));
    ui->actionNext_Day->setToolTip(tr("Next Day"));

    ui->actionPrevious_Day->setText(tr("Previous Day"));
    ui->actionPrevious_Day->setToolTip(tr("Previous Day"));

    ui->actionNext_Month->setText(tr("Next Month"));
    ui->actionNext_Month->setToolTip(tr("Next Month"));

    ui->actionPrevious_Month->setText(tr("Previous Month"));
    ui->actionPrevious_Month->setToolTip(tr("Previous Month"));

    ui->actionShow_Day_Events->setText(tr("Show Day Events"));
    ui->actionShow_Day_Events->setToolTip(tr("Show Day Events"));

    ui->actionToday->setText(tr("Today (Spacebar)"));
    ui->actionToday->setToolTip(tr("Today"));

    ui->action_Increase_Font->setText(tr("Increase Calendar Font Size"));
    ui->action_Increase_Font->setToolTip(tr("Increase Calendar Font Size"));

    ui->actionDecrease_Font->setText(tr("Decrease Calendar Font Size"));
    ui->actionDecrease_Font->setToolTip(tr("Decrease Calendar Font Size"));

    ui->actionReset_Calendar_Font_Size->setText(tr("Reset Calendar Font Size"));
    ui->actionReset_Calendar_Font_Size->setToolTip(tr("Reset Calendar Font Size"));

    //Options menu

    ui->menuOptions->setTitle(tr("&Options"));
   // ui->menuOptions->setToolTip(tr("Options"));


    ui->actionShow_Birthdays_on_Calendar->setText(tr("Show Birthdays On Calendar"));
    ui->actionShow_Birthdays_on_Calendar->setToolTip(tr("Show Birthdays On Calendar"));

    ui->actionShow_Holidays_on_Calendar->setText(tr("Show Holidays On Calendar"));
    ui->actionShow_Holidays_on_Calendar->setToolTip(tr("Show Holidays On Calendar"));

    ui->actionColour_Code_Appointments->setText(tr("Colour Code Calendar"));
    ui->actionColour_Code_Appointments->setToolTip(tr("Colour Code Calendar"));

    ui->actionShow_General_Events->setText(tr("Show General"));
    ui->actionShow_General_Events->setToolTip(tr("Show General"));

    ui->actionShow_Family_Events->setText(tr("Show Family"));
    ui->actionShow_Family_Events->setToolTip(tr("Show Family"));

    ui->actionShow_Leisure_Events->setText(tr("Show Leisure"));
    ui->actionShow_Leisure_Events->setToolTip(tr("Show Leisure"));

    ui->actionShow_Meetings->setText(tr("Show Meetings"));
    ui->actionShow_Meetings->setToolTip(tr("Show Meetings"));

    ui->actionShow_Work->setText(tr("Show Work"));
    ui->actionShow_Work->setToolTip(tr("Show Work"));

    ui->actionShow_Vacations->setText(tr("Show Vacations"));
    ui->actionShow_Vacations->setToolTip(tr("Show Vacations"));

    ui->actionShow_Medical->setText(tr("Show Medical"));
    ui->actionShow_Medical->setToolTip(tr("Show Medical"));

    ui->actionShow_Fitness->setText(tr("Show Fitness"));
    ui->actionShow_Fitness->setToolTip(tr("Show Fitness"));



    ui->actionShow_Birthdays_on_Calendar->setChecked(true);
    ui->actionShow_Holidays_on_Calendar->setChecked(true);
    ui->actionColour_Code_Appointments->setChecked(true);
    ui->actionShow_General_Events->setChecked(true);
    ui->actionShow_Family_Events->setChecked(true);
    ui->actionShow_Leisure_Events->setChecked(true);
    ui->actionShow_Meetings->setChecked(true);
    ui->actionShow_Work->setChecked(true);
    ui->actionShow_Vacations->setChecked(true);
    ui->actionShow_Medical->setChecked(true);
    ui->actionShow_Fitness->setChecked(true);
    ui->actionColour_Code_Appointments->setChecked(true);

    //Help menu

    ui->menuHelp->setTitle(tr("&Help"));
    //ui->menuHelp->setToolTip(tr("Help"));

    ui->actionAbout->setText(tr("About"));
    ui->actionAbout->setToolTip(tr("About"));

    ui->pushButtonShowQuickFullView->setText(tr("Quick/Full View"));
    ui->pushButtonShowQuickFullView->setToolTip(tr("Quick/Full View"));

    ui->pushButtonMailTo->setText(tr("Mail To:"));
    ui->pushButtonMailTo->setToolTip(tr("Mail To:"));

    //Keyboard QActions (shortcuts)
    gotoNextDayAction= new QAction(this);
    gotoNextDayAction->setShortcut(Qt::Key_Right);
    connect(gotoNextDayAction, SIGNAL(triggered()), this, SLOT(gotoNextDaySlot()));
    this->addAction(gotoNextDayAction);

    gotoPreviousDayAction= new QAction(this);
    gotoPreviousDayAction->setShortcut(Qt::Key_Left);
    connect(gotoPreviousDayAction, SIGNAL(triggered()), this, SLOT(gotoPreviousDaySlot()));
    this->addAction(gotoPreviousDayAction);

    gotoNextMonthAction= new QAction(this);
    gotoNextMonthAction->setShortcut(Qt::Key_Up);
    connect(gotoNextMonthAction, SIGNAL(triggered()), this, SLOT(gotoNextMonthSlot()));
    this->addAction(gotoNextMonthAction);

    gotoPreviousMonthAction= new QAction(this);
    gotoPreviousMonthAction->setShortcut(Qt::Key_Down);
    connect(gotoPreviousMonthAction, SIGNAL(triggered()), this, SLOT(gotoPreviousMonthSlot()));
    this->addAction(gotoPreviousMonthAction);

    gotoTodayAction= new QAction(this);
    gotoTodayAction->setShortcut(Qt::Key_Space);
    connect(gotoTodayAction, SIGNAL(triggered()), this, SLOT(gotoTodaySlot()));
    this->addAction(gotoTodayAction);

    showAppointmentDetailsAction= new QAction(this);
    showAppointmentDetailsAction->setShortcut(Qt::Key_Return);
    connect(showAppointmentDetailsAction, SIGNAL(triggered()),
            this, SLOT(showAppointmentDetailsSlot()));
    this->addAction(showAppointmentDetailsAction);

    increaseFontAction=new QAction(this);
    increaseFontAction->setShortcut(Qt::Key_F);
    connect(increaseFontAction, SIGNAL(triggered()),
            this, SLOT(increaseFontSlot()));
    this->addAction(increaseFontAction);

    resetFontAction=new QAction(this);
    resetFontAction->setShortcut(Qt::Key_R);
    connect(resetFontAction, SIGNAL(triggered()),
            this, SLOT(resetFontSlot()));
    this->addAction(resetFontAction);

    decreaseFontAction=new QAction(this);
    decreaseFontAction->setShortcut(Qt::Key_D);
    connect(decreaseFontAction, SIGNAL(triggered()),
            this, SLOT(decreaseFontSlot()));
    this->addAction(decreaseFontAction);

    newAppointmentAction=new QAction(this);
    newAppointmentAction->setShortcut(Qt::Key_A);
    connect(newAppointmentAction, SIGNAL(triggered()),
            this, SLOT(newAppointmentSlot()));
    this->addAction(newAppointmentAction);

    newContactAction=new QAction(this);
    newContactAction->setShortcut(Qt::Key_C);
    connect(newContactAction, SIGNAL(triggered()),
            this, SLOT(newContactSlot()));
    this->addAction(newContactAction);


    //Setup empty lists
    appointmentList= QList<Appointment>();

    holidayList=QList<Holiday>();

    contactList =QList<Contact>();

    AddHolidaysToHolidayList(selectedYear);

    LoadDatabaseAppointmentsToAppointmentList();

    //Add contacts to contactList
    contactModel = new ContactModel; //model view
    proxyModelContacts= new ProxyModelContacts;
    LoadDatebaseContactsToContactList(); //loads contactlist
    DisplayContactsOnTableView();

    fontSize = 12;
    UpdateCalendar();

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

        title =appointmentDialog->getTitle();
        location =appointmentDialog->getLocation();
        description= appointmentDialog->getDescription();
        selectedDate=appointmentDialog->getAppointmentDate(); //appointment date
        appointmentStartTime=appointmentDialog->getStartTime();
        appointmentEndTime=appointmentDialog->getEndTime();
        category=appointmentDialog->getCategory();     
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

        if (dbm.isOpen())
        {
            appointmentId=dbm.addAppointment(a);           
            appointmentList.append(a);
            //qDebug()<<"Added Appointment: Appointment ID = "<<appointmentId;
            UpdateCalendar();
        }        
    }
}

void MainWindow::GenerateRepeatAppointments()
{
    DialogRepeatAppointment *repeatDialog = new  DialogRepeatAppointment(this,&selectedDate);
    repeatDialog->setModal(true);

    if (repeatDialog->exec() == QDialog::Accepted ) {

        title =repeatDialog->getTitle();
        location =repeatDialog->getLocation();
        description= repeatDialog->getDescription();
        selectedDate=repeatDialog->getAppointmentDate(); //appointment date
        appointmentStartTime=repeatDialog->getStartTime();
        appointmentEndTime=repeatDialog->getEndTime();
        category=repeatDialog->getCategory();
        isAllDay=repeatDialog->getAllDay();

        repeatDayInterval=repeatDialog->getRepeatDayInterval();
        repeatNumber=repeatDialog->getRepeatNumber();

        Appointment a;
        a.m_title=title;
        a.m_location=location;
        a.m_description=description;
        a.m_date=selectedDate.toString();
        a.m_startTime=appointmentStartTime.toString();
        a.m_endTime=appointmentEndTime.toString();
        a.m_category=category;
        a.m_isFullDay=isAllDay;


        Appointment tmp;
        QDate repeatDate=QDate::fromString(a.m_date);

        for (int i=0;i<repeatNumber;i+= 1)
        {
            tmp.m_title=a.m_title;
            //tmp.m_title=a.m_title;
            tmp.m_location=a.m_location;
            tmp.m_description=a.m_description;
            tmp.m_date=repeatDate.addDays(repeatDayInterval*(i)).toString();
            tmp.m_startTime=a.m_startTime;
            tmp.m_endTime=a.m_endTime;
            tmp.m_category=a.m_category;
            tmp.m_isFullDay=a.m_isFullDay;


            if (dbm.isOpen())
            {
                appointmentId=dbm.addAppointment(tmp);
                //qDebug()<<"Repeat Appointment ID = "<<appointmentId;
                appointmentList.append(tmp);
                //qDebug()<<"Added Appointment: Appointment ID = "<<appointmentId;
            }
        }
        UpdateCalendar();
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



void MainWindow::LoadDatabaseAppointmentsToAppointmentList()
{
    appointmentList.clear();
    QList<Appointment> tmpList = dbm.getAllAppointments();
    foreach(Appointment a, tmpList)
    {
        appointmentList.append(a);
        //qDebug()<<"LoadDBAppointmentToAppointmentList: "<<a.m_title<<a.m_date;
    }
}


void MainWindow::LoadDatebaseContactsToContactList()
{
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


    DialogContact *contactDialog = new DialogContact(this);
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
    }
    DisplayContactsOnTableView(); //display
    UpdateCalendar();
}

void MainWindow::UpdateContact(int dbID)
{

    Contact currentContact = dbm.getContactByID(dbID);    
    DialogContact *contactDialog = new  DialogContact(&currentContact,this);

    contactDialog->setModal(true);
    if (contactDialog->exec() == QDialog::Accepted ) {

        if(contactDialog->getDeleteRequested())
        {
            dbm.removeContactById(dbID);
            RemoveContactFromContactList(dbID);
            DisplayContactsOnTableView();
            UpdateCalendar();
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
            LoadDatebaseContactsToContactList();
        }
    }
    DisplayContactsOnTableView();
    UpdateCalendar(); //Birthdays
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
   // ui->tableViewContacts->hideColumn(11); //telephone
    //birthday
    ui->tableViewContacts->hideColumn(12); //birthday
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

void MainWindow::checkForBirthdaysNextSevenDays()
{
    DialogUpcomingBirthdays  *birthdayDialog =
            new  DialogUpcomingBirthdays(this,&contactList);
    birthdayDialog->setModal(true);

    if (birthdayDialog->exec() == QDialog::Accepted ) {
    }
}

void MainWindow::checkAppointmentsNextSevenDays()
{
    DialogUpcomingSchedule  *scheduleDialog =
            new  DialogUpcomingSchedule(this, &dbm);
    scheduleDialog->setModal(true);

    if (scheduleDialog->exec() == QDialog::Accepted ) {
    }
}

void MainWindow::increaseFont()
{
    fontSize =fontSize+1;
    if (fontSize >30) fontSize =30;
    UpdateCalendar();
}

void MainWindow::decreaseFont()
{
    fontSize=fontSize-1;
    if (fontSize<8) fontSize=8;
    UpdateCalendar();
}

void MainWindow::resetFont()
{
    fontSize=12;
    UpdateCalendar();
}

void MainWindow::gotoNextDay()
{
    int daysInMonth=selectedDate.daysInMonth();
    selectedMonth =selectedDate.month();
    selectedYear =selectedDate.year();
    selectedDay =selectedDate.day()+1; //add day move forward

    if (selectedDay > daysInMonth) {
        selectedMonth =selectedMonth+1;
        selectedDay=1;
        if (selectedMonth >= 13) {
            selectedMonth = 1;
            selectedYear =selectedYear+1;
            AddHolidaysToHolidayList(selectedYear);
            UpdateCalendar();
            selectedDate.setDate(selectedYear,selectedMonth,selectedDay);
            selectedDateLabel->setText(selectedDate.toString());
        }
    }

    selectedDate.setDate(selectedYear,selectedMonth,selectedDay);   
    UpdateCalendar();
    selectedDateLabel->setText(selectedDate.toString());
}

void MainWindow::gotoPreviousDay()
{
    selectedMonth =selectedDate.month();
    selectedYear =selectedDate.year();
    selectedDay =selectedDate.day()-1;

    if (selectedDay < 1) {
        selectedMonth = selectedMonth-1;
        if (selectedMonth <= 1)
        {
            selectedMonth = 12;
            selectedYear=selectedYear-1;
            AddHolidaysToHolidayList(selectedYear);
            UpdateCalendar();
            selectedDate.setDate(selectedYear,selectedMonth,selectedDay);
            selectedDateLabel->setText(selectedDate.toString());
        }

        QDate tmp =QDate(selectedYear, selectedMonth,1);
        int daysInMonth =tmp.daysInMonth();
        selectedDay=daysInMonth;
    }
    selectedDate.setDate(selectedYear,selectedMonth,selectedDay);
    UpdateCalendar();
    selectedDateLabel->setText(selectedDate.toString()); 
}

void MainWindow::gotoNextMonth()
{
    selectedMonth =selectedDate.month()+1;
    selectedYear =selectedDate.year();    
    selectedDay=1;

    if (selectedMonth >= 13) {
        selectedMonth = 1;
        selectedYear =selectedYear+1;
        AddHolidaysToHolidayList(selectedYear);
        UpdateCalendar();
        selectedDate.setDate(selectedYear,selectedMonth,selectedDay);
        selectedDateLabel->setText(selectedDate.toString());
    }
    selectedDate.setDate(selectedYear,selectedMonth,selectedDay);
    UpdateCalendar();
    selectedDateLabel->setText(selectedDate.toString());
}

void MainWindow::gotoPreviousMonth()
{
    selectedMonth =selectedDate.month()-1;
    selectedYear =selectedDate.year();    
    selectedDay=1;

    if (selectedMonth < 1)
    {
        selectedMonth = 12;
        selectedYear=selectedYear-1;
        AddHolidaysToHolidayList(selectedYear);
        UpdateCalendar();
        selectedDate.setDate(selectedYear,selectedMonth,selectedDay);
        selectedDateLabel->setText(selectedDate.toString());
    }
    selectedDate.setDate(selectedYear,selectedMonth,selectedDay);
    UpdateCalendar();
    selectedDateLabel->setText(selectedDate.toString()); 
}

void MainWindow::gotoToday()
{
    selectedDate=QDate::currentDate();
    selectedDay=selectedDate.day();
    selectedMonth=selectedDate.month();
    selectedYear=selectedDate.year();
    AddHolidaysToHolidayList(selectedYear);
    UpdateCalendar();
    selectedDateLabel->setText(selectedDate.toString());
}

void MainWindow::showDayEvents()
{
    DialogShowDayDetails *showDetailsDialog =
            new  DialogShowDayDetails(this,&selectedDate, &dbm);
    showDetailsDialog->setModal(true);

    if (showDetailsDialog->exec() == QDialog::Accepted ) {

    }    
    LoadDatabaseAppointmentsToAppointmentList();
    UpdateCalendar();
}

void MainWindow::ExportContactsXML()
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
        QString addToCal =QString::number(c.m_addToCalendar);

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
        contact.setAttribute("AddToCal",addToCal);
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

void MainWindow::ImportContactsXML()
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
            c.m_addToCalendar=contact.attribute("AddToCal").toInt();

            if (dbm.isOpen())
            {
                contactId=dbm.addContact(c);
                //qDebug()<<"Added contact contactId = "<<contactId;
            }
        }
    }
    file.close();    
    LoadDatebaseContactsToContactList(); //refresh contactslist here
    DisplayContactsOnTableView(); //display
    UpdateCalendar(); //show birthdays
}

void MainWindow::ExportAppointmentsXML()
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
        QString category=a.m_category;
        QString allDay =QString::number(a.m_isFullDay);

        QDomElement appointment = document.createElement("Appointment");
        appointment.setAttribute("Title",title);
        appointment.setAttribute("Location", location);
        appointment.setAttribute("Description",description);
        appointment.setAttribute("Date",date);
        appointment.setAttribute("StartTime",startTime);
        appointment.setAttribute("EndTime",endTime);
        appointment.setAttribute("Category",category);
        appointment.setAttribute("IsAllDay",allDay);
        root.appendChild(appointment);

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

void MainWindow::ImportAppointmentsXML()
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
            a.m_category=appointment.attribute("Category");
            a.m_isFullDay=appointment.attribute("IsAllDay").toInt();

            if (dbm.isOpen())
            {
                dbm.addAppointment(a);
            }
        }
    }

    file.close();
    //qDebug() << "Import Finished";
    LoadDatabaseAppointmentsToAppointmentList();
    UpdateCalendar();
}

void MainWindow::gotoNextDaySlot()
{
    gotoNextDay();
}

void MainWindow::gotoPreviousDaySlot()
{
    gotoPreviousDay();
}

void MainWindow::gotoNextMonthSlot()
{
    gotoNextMonth();
}

void MainWindow::gotoPreviousMonthSlot()
{
    gotoPreviousMonth();
}

void MainWindow::gotoTodaySlot()
{
    gotoToday();
}

void MainWindow::showAppointmentDetailsSlot()
{
    showDayEvents();
}

void MainWindow::increaseFontSlot()
{
    increaseFont();
}

void MainWindow::decreaseFontSlot()
{
    decreaseFont();
}

void MainWindow::resetFontSlot()
{
    resetFont();
}

void MainWindow::newAppointmentSlot()
{
    NewAppointment();
}

void MainWindow::newContactSlot()
{
    NewContact();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::UpdateCalendar()
{
    ui->textBrowserCalendar->clear();

       QTextCursor cursor = ui->textBrowserCalendar->textCursor();
       cursor.beginEditBlock();
       QDate date(selectedDate.year(), selectedDate.month(), 1);

       QTextTableFormat tableFormat;
       tableFormat.setAlignment(Qt::AlignHCenter);
       tableFormat.setBackground(QColor("#f0f0f0"));
       tableFormat.setCellPadding(2);
       tableFormat.setCellSpacing(4);
       QVector<QTextLength> constraints;
       constraints << QTextLength(QTextLength::PercentageLength, 14)
                   << QTextLength(QTextLength::PercentageLength, 14)
                   << QTextLength(QTextLength::PercentageLength, 14)
                   << QTextLength(QTextLength::PercentageLength, 14)
                   << QTextLength(QTextLength::PercentageLength, 14)
                   << QTextLength(QTextLength::PercentageLength, 14)
                   << QTextLength(QTextLength::PercentageLength, 14);
       tableFormat.setColumnWidthConstraints(constraints);

       QTextTable *table = cursor.insertTable(1, 7, tableFormat);
       QTextFrame *frame = cursor.currentFrame();

       QTextFrameFormat frameFormat = frame->frameFormat();
       frameFormat.setBorder(1);
       frame->setFrameFormat(frameFormat);

       QTextCharFormat format = cursor.charFormat();
       format.setFontPointSize(fontSize);
       format.setForeground(Qt::black);

       QTextCharFormat formatBlue = cursor.charFormat();
       formatBlue.setFontPointSize(fontSize);
       formatBlue.setForeground(Qt::blue);

       QTextCharFormat formatDarkGray = cursor.charFormat();
       formatDarkGray.setFontPointSize(fontSize);
       formatDarkGray.setForeground(Qt::darkGray);

       QTextCharFormat formatDarkBlue = cursor.charFormat();
       formatDarkBlue.setFontPointSize(fontSize);
       formatDarkBlue.setForeground(Qt::darkBlue);

       QTextCharFormat formatDarkMagenta = cursor.charFormat();
       formatDarkMagenta.setFontPointSize(fontSize);
       formatDarkMagenta.setForeground(Qt::darkMagenta);

       QTextCharFormat formatMagenta = cursor.charFormat();
       formatMagenta.setFontPointSize(fontSize);
       formatMagenta.setForeground(Qt::magenta);

       QTextCharFormat formatDarkYellow = cursor.charFormat();
       formatDarkYellow.setFontPointSize(fontSize);
       formatDarkYellow.setForeground(Qt::darkYellow);


       QTextCharFormat formatDarkGreen = cursor.charFormat();
       formatDarkGreen.setFontPointSize(fontSize);
       formatDarkGreen.setForeground(Qt::darkGreen);

//       QTextCharFormat formatGreen = cursor.charFormat();
//       formatGreen.setFontPointSize(fontSize);
//       formatGreen.setForeground(Qt::green);

       QTextCharFormat formatDarkCyan = cursor.charFormat();
       formatDarkCyan .setFontPointSize(fontSize);
       formatDarkCyan .setForeground(Qt::darkCyan);

       QTextCharFormat formatBlack = cursor.charFormat();
       formatBlack.setFontPointSize(fontSize);
       formatBlack.setForeground(Qt::black);

       QTextCharFormat formatRed = cursor.charFormat();
       formatRed.setFontPointSize(fontSize);
       //formatBirthday.setForeground(Qt::darkGreen);
       formatRed.setForeground(Qt::red);


       QTextCharFormat formatSelectedDay = cursor.charFormat();
       formatSelectedDay.setFontPointSize(fontSize);
       formatSelectedDay.setBackground(Qt::green);


       QTextCharFormat boldFormat = format;
       boldFormat.setFontWeight(QFont::Bold);
       boldFormat.setForeground(Qt::black);

       QTextCharFormat highlightedFormat = boldFormat;
       highlightedFormat.setBackground(Qt::yellow);

       for (int weekDay = 1; weekDay <= 7; ++weekDay) {
           QTextTableCell cell = table->cellAt(0, weekDay-1);
           QTextCursor cellCursor = cell.firstCursorPosition();
           cellCursor.insertText(QLocale::system().dayName(weekDay), boldFormat);

       }

       table->insertRows(table->rows(), 1);

       while (date.month() == selectedDate.month()) {
           int weekDay = date.dayOfWeek();
           QTextTableCell cell = table->cellAt(table->rows()-1, weekDay-1);
           QTextCursor cellCursor = cell.firstCursorPosition();

           if (date == QDate::currentDate())
           {
               cellCursor.insertText(QString(tr("%1")).arg(date.day()), highlightedFormat);

           }
           else if(date==selectedDate){
               cellCursor.insertText(QString(tr("%1")).arg(date.day()), formatSelectedDay);
           }
           else
               cellCursor.insertText(QString(tr("%1")).arg(date.day()), format);

           //-----------------------------------------------------
           // Add holidays to calendar
           //-----------------------------------------------------
           if(flagShowHolidays){

               foreach (Holiday h, holidayList)
               {
                   QDate d =QDate::fromString(h.m_date);

                   if(date ==d)
                   {
                       if(flagColourCoding){
                           cellCursor.insertText("\n"+h.m_name, formatDarkMagenta);
                       }
                       else {
                           cellCursor.insertText("\n"+h.m_name, formatBlack);
                       }
                   }
               }
           }

            //-----------------------------------------------------
           //Add birthdays using contactList
           //-----------------------------------------------------
           if(flagShowBirthdays)
           {
               foreach (Contact c, contactList)
               {
                   QDate birth =QDate::fromString(c.m_birthdate);
                   QDate birthday =QDate(selectedYear,birth.month(),birth.day());
                   QString name =c.m_firstname+" "+c.m_lastname;

                   if((date ==birthday) && (c.m_addToCalendar ==1))
                   {

                       if(flagColourCoding){
                           cellCursor.insertText(tr("\nBirthday: ")+name, formatDarkCyan);
                       }
                       else {
                           cellCursor.insertText(tr("\nBirthday: ")+name, formatBlack);
                       }

                   }
               }
           }


           //-----------------------------------------------------
           //Add appointments to calendar
           //-----------------------------------------------------

           QList<Appointment> dayList = *new QList<Appointment>();

           foreach(Appointment a, appointmentList)
           {
               QDate d =QDate::fromString(a.m_date);

               if(date ==d)
               {
                dayList.append(a);
               }

           }

           std::sort(dayList.begin(), dayList.end(), compare);

           foreach(Appointment a, dayList)
           {
               if(a.m_category=="General" && flagShowGeneralEvents)
               {
                   if(flagColourCoding){
                       cellCursor.insertText("\n"+a.m_title, formatDarkBlue);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }
               else if(a.m_category=="Meeting" && flagShowMeetings){
                   if(flagColourCoding){
                       cellCursor.insertText("\n"+a.m_title, formatBlue);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }
               else if(a.m_category=="Work" && flagShowWorkEvents){
                   if(flagColourCoding){
                       cellCursor.insertText("\n"+a.m_title, formatBlue);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }
               else if(a.m_category=="Family" && flagShowFamilyEvents){

                   if(flagColourCoding) {
                       cellCursor.insertText("\n"+a.m_title, formatMagenta);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }
               else if(a.m_category=="Leisure" && flagShowLeisureEvents){
                   if(flagColourCoding){
                       cellCursor.insertText("\n"+a.m_title, formatDarkYellow);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }
               else if(a.m_category=="Fitness" && flagShowFitness){
                   if(flagColourCoding){
                       cellCursor.insertText("\n"+a.m_title, formatDarkGreen);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }


               else if(a.m_category=="Vacation" && flagShowVacations){
                   if(flagColourCoding){
                       cellCursor.insertText("\n"+a.m_title, formatDarkGray);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }

               else if(a.m_category=="Medical" && flagShowMedical){
                   if(flagColourCoding){
                       cellCursor.insertText("\n"+a.m_title, formatRed);
                   }
                   else {
                       cellCursor.insertText("\n"+a.m_title, formatBlack);
                   }
               }
           }
           date = date.addDays(1);
           if (weekDay == 7 && date.month() == selectedDate.month())
               table->insertRows(table->rows(), 1);
       }
       cursor.endEditBlock();

       ui->labelMonthYear->setText
               (tr("%1 %2"
                   ).arg(QLocale::system().monthName(selectedDate.month())
                         ).arg(selectedDate.year()));

}

void MainWindow::AddHolidaysToHolidayList(int year)
{
    holidayList.clear();

    Holiday h1;
    h1.m_id=1;
    h1.m_name=tr("Christmas");
    h1.m_date=QDate(year,12,25).toString();
    h1.m_addToCalendar=1;
    holidayList.append(h1);

    Holiday h2;
    h2.m_id=2;
    h2.m_name=tr("Boxing Day");
    h2.m_date=QDate(year,12,26).toString();
    h2.m_addToCalendar=1;
    holidayList.append(h2);

    Holiday h3;
    h3.m_id=3;
    h3.m_name=tr("New Years Day");
    h3.m_date=QDate(year,1,1).toString();
    h3.m_addToCalendar=1;
    holidayList.append(h3);

    QDate easterSunday =CalculateEaster(year);

    Holiday h4;
    h4.m_id=4;
    h4.m_name=tr("Easter");
    h4.m_date=easterSunday.toString();
    h4.m_addToCalendar=1;
    holidayList.append(h4);

    Holiday h5;
    h5.m_id=5;
    h5.m_name=tr("Good Friday");
    h5.m_date=easterSunday.addDays(-2).toString();
    h5.m_addToCalendar=1;
    holidayList.append(h5);

    Holiday h6;
    h6.m_id=6;
    h6.m_name=tr("Easter Monday");
    h6.m_date=easterSunday.addDays(1).toString();
    h6.m_addToCalendar=1;
    holidayList.append(h6);

    Holiday h7;
    h7.m_id=7;
    h7.m_name=tr("May Bank Holiday");
    QDate mayDate =QDate(year,5,1);
    int dayOfWeek = mayDate.dayOfWeek();
    int firstMonday = mayDate.day()-dayOfWeek+1;
    firstMonday = (firstMonday <= 0) ? firstMonday + 7 : firstMonday % 7;
    h7.m_date=QDate(year,5,firstMonday).toString();
    h7.m_addToCalendar=1;
    holidayList.append(h7);

    //Days in May

    int daysInMay = mayDate.daysInMonth();
    int plusDays=0;

    if (firstMonday+28<=daysInMay){

        plusDays =28;
    }
    else {
        plusDays=21;
    }


    Holiday h8;
    h8.m_id=8;
    h8.m_name=tr("Spring Bank Holiday");
    QDate springbank =QDate(year,5,firstMonday).addDays(plusDays);

    if (springbank.isValid())
    {
    h8.m_date=springbank.toString();
    h8.m_addToCalendar=1;
    holidayList.append(h8);
    }

//    int weeksInMay = QDate(year, 5, daysInMonth).weekNumber()
//        - QDate(year, 5, 1).weekNumber() + 1;


    Holiday h9;
    h9.m_id=9;
    h9.m_name=tr("Summer Bank Holiday");
    QDate augustDate =QDate(year,8,1);
    int dayOfWeekAug = augustDate.dayOfWeek();
    int firstMondayAug = mayDate.day()-dayOfWeekAug+1;
    firstMondayAug = (firstMondayAug <= 0) ? firstMondayAug + 7 : firstMondayAug % 7;

    //Days in May

    int daysInAugust = augustDate.daysInMonth();
    plusDays=0;

    if (firstMondayAug+28<=daysInAugust){

        plusDays =28;
    }
    else {
        plusDays=21;
    }

    QDate summerBankHol =QDate(year,8,firstMondayAug).addDays(plusDays);

    if(summerBankHol.isValid())
    {
        h9.m_date=summerBankHol.toString();
        h9.m_addToCalendar=1;
        holidayList.append(h9);

    }
}


void MainWindow::on_actionNext_Day_triggered()
{
    gotoNextDay();
}

void MainWindow::on_actionPrevious_Day_triggered()
{
    gotoPreviousDay();
}

void MainWindow::on_actionNext_Month_triggered()
{
    gotoNextMonth();
}

void MainWindow::on_actionPrevious_Month_triggered()
{
    gotoPreviousMonth();
}

void MainWindow::on_actionNew_Appointment_triggered()
{
    NewAppointment();
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

void MainWindow::on_actionCheck_For_Birthdays_triggered()
{
    checkForBirthdaysNextSevenDays();
}

void MainWindow::on_pushButtonShowQuickFullView_clicked()
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
        //ui->tableViewContacts->hideColumn(11); //telephone
        ui->tableViewContacts->hideColumn(12); //birthday
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
        ui->tableViewContacts->showColumn(12); //birthday
        flagQuickView=true;
    }
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
    //qDebug()<<"SelectedContactId = "<<selectedCont
    selectedContact=dbm.getContactByID(selectedContactDbId);
}

void MainWindow::on_actionShow_Birthdays_on_Calendar_triggered()
{
    if(ui->actionShow_Birthdays_on_Calendar->isChecked())
    {
        ui->actionShow_Birthdays_on_Calendar->setChecked(true);
        flagShowBirthdays=true;

    }
    else {
        ui->actionShow_Birthdays_on_Calendar->setChecked(false);
        flagShowBirthdays=false;
    }
    UpdateCalendar();
}

void MainWindow::on_actionShow_Holidays_on_Calendar_triggered()
{
    if(ui->actionShow_Holidays_on_Calendar->isChecked())
    {
       ui->actionShow_Holidays_on_Calendar->setChecked(true);
       flagShowHolidays=true;
    }
    else {
       ui->actionShow_Holidays_on_Calendar->setChecked(false);
       flagShowHolidays=false;
    }

    UpdateCalendar();

}

void MainWindow::on_actionExport_Contacts_XML_triggered()
{
    ExportContactsXML();
}

void MainWindow::on_actionImport_Contacts_XML_triggered()
{
    ImportContactsXML();
}

void MainWindow::on_actionDelete_All_Appointments_triggered()
{
    dbm.deleteAllAppointments();
    appointmentList.clear();
    UpdateCalendar();

}

void MainWindow::on_actionDelete_All_Contacts_triggered()
{
    dbm.removeAllContacts();
    contactList.clear();
    DisplayContactsOnTableView();
    UpdateCalendar();
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout *aboutDialog = new DialogAbout(this);
    aboutDialog->setModal(false);
    aboutDialog->exec();
}

void MainWindow::on_actionShow_Day_Events_triggered()
{
   showDayEvents();
}

void MainWindow::on_action_Increase_Font_triggered()
{
    increaseFont();

}

void MainWindow::on_actionDecrease_Font_triggered()
{
    decreaseFont();


}

void MainWindow::on_actionReset_Calendar_Font_Size_triggered()
{
    resetFont();

}

void MainWindow::on_actionUpcoming_Schedule_triggered()
{
    checkAppointmentsNextSevenDays();
}

void MainWindow::on_actionToday_triggered()
{
    gotoToday();
}

void MainWindow::on_actionGenerate_Repeat_Appointments_triggered()
{
    GenerateRepeatAppointments();
}


void MainWindow::on_actionShow_General_Events_triggered()
{


    if(ui->actionShow_General_Events->isChecked())
    {
        ui->actionShow_General_Events->setChecked(true);
        flagShowGeneralEvents=true;
    }
    else {
        ui->actionShow_General_Events->setChecked(false);
        flagShowGeneralEvents=false;
    }
    UpdateCalendar();
}

void MainWindow::on_actionShow_Family_Events_triggered()
{
    if(ui->actionShow_Family_Events->isChecked())
    {
        ui->actionShow_Family_Events->setChecked(true);
        flagShowFamilyEvents=true;
    }
    else {
        ui->actionShow_Family_Events->setChecked(false);
        flagShowFamilyEvents=false;
    }
    UpdateCalendar();

}

void MainWindow::on_actionShow_Leisure_Events_triggered()
{
    if(ui->actionShow_Leisure_Events->isChecked())
    {
        ui->actionShow_Leisure_Events->setChecked(true);
        flagShowLeisureEvents=true;
    }
    else {
        ui->actionShow_Leisure_Events->setChecked(false);
        flagShowLeisureEvents=false;
    }
    UpdateCalendar();

}

void MainWindow::on_actionShow_Meetings_triggered()
{
    if(ui->actionShow_Meetings->isChecked())
    {
        ui->actionShow_Meetings->setChecked(true);
        flagShowMeetings=true;
    }
    else {
        ui->actionShow_Meetings->setChecked(false);
        flagShowMeetings=false;
    }
    UpdateCalendar();

}

void MainWindow::on_actionShow_Work_triggered()
{
    if(ui->actionShow_Work->isChecked())
    {
        ui->actionShow_Work->setChecked(true);
        flagShowWorkEvents=true;
    }
    else {
        ui->actionShow_Work->setChecked(false);
        flagShowWorkEvents=false;
    }
    UpdateCalendar();

}

void MainWindow::on_actionShow_Vacations_triggered()
{
    if(ui->actionShow_Vacations->isChecked())
    {
        ui->actionShow_Vacations->setChecked(true);
        flagShowVacations=true;
    }
    else {
        ui->actionShow_Vacations->setChecked(false);
        flagShowVacations=false;
    }
    UpdateCalendar();

}

void MainWindow::on_actionShow_Medical_triggered()
{
    if(ui->actionShow_Medical->isChecked())
    {
        ui->actionShow_Medical->setChecked(true);
        flagShowMedical=true;
    }
    else {
        ui->actionShow_Medical->setChecked(false);
        flagShowMedical=false;
    }
    UpdateCalendar();

}

void MainWindow::on_actionColour_Code_Appointments_triggered()
{
    if(ui->actionColour_Code_Appointments->isChecked())
    {
        ui->actionColour_Code_Appointments->setChecked(true);
        flagColourCoding=true;
    }
    else {
        ui->actionColour_Code_Appointments->setChecked(false);
        flagColourCoding=false;
    }
    UpdateCalendar();

}

void MainWindow::on_actionShow_Fitness_triggered()
{
    if(ui->actionShow_Fitness->isChecked()){

        ui->actionShow_Fitness->setChecked(true);
        flagShowFitness=true;
    }
    else {
        ui->actionShow_Fitness->setChecked(false);
        flagShowFitness=false;
    }
    UpdateCalendar();
}

void MainWindow::on_actionExport_Appointments_XML_triggered()
{
    ExportAppointmentsXML();
}

void MainWindow::on_actionImport_Appointments_XML_triggered()
{
    ImportAppointmentsXML();
}
