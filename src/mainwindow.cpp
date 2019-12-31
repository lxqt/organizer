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
    //set layout first
    QMainWindow::centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    //Setup database
    dbm.openDatabase();
    dbm.createDatebaseTables();

    selectedDate = QDate::currentDate();

    dateInfoLabel =new QLabel(this);
    dateInfoLabel->setText("Selected Date: ");
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

    ui->actionShow_Birthdays_on_Calendar->setChecked(true);
    ui->actionShow_Holidays_on_Calendar->setChecked(true);

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
        a.m_isRepeating=0;
        a.m_parentId=0;


        if (dbm.isOpen())
        {
            appointmentId=dbm.addAppointment(a);           
            appointmentList.append(a);
            //qDebug()<<"Added Appointment: Appointment ID = "<<appointmentId;
            UpdateCalendar();
        }
    }
}



void MainWindow::LoadDatabaseAppointmentsToAppointmentList()
{
    appointmentList.clear();
    QList<Appointment> tmpList = dbm.getAllAppointments();
    foreach(Appointment a, tmpList)
    {
        appointmentList.append(a);
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
           this->addBirthdayToCalendar=contactDialog->getAddToCal();


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

           qDebug()<<"Update Contact: AddToCal = "<<c.m_addToCalendar;

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
    DialogContact *contactDialog = new  DialogContact(this,&currentContact);

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
        this->addBirthdayToCalendar=contactDialog->getAddToCal();

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
            //UpdateContactInContactList(c,dbID);
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
    ui->tableViewContacts->hideColumn(11); //telephone
    //birthday
    ui->tableViewContacts->hideColumn(13); //birthID
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

    //Check contactList
    qDebug()<<"ContactList Size = "<<contactList.size();

    foreach(Contact c, contactList)
    {
        qDebug()<<"ContactList: "<<c.m_lastname<<" AddToCal = "<<c.m_addToCalendar;
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

       QTextCharFormat formatBlue = cursor.charFormat();
       formatBlue.setFontPointSize(fontSize);
       formatBlue.setForeground(Qt::blue);

//       QTextCharFormat formatDarkBlue = cursor.charFormat();
//       formatDarkBlue.setFontPointSize(fontSize);
//       formatDarkBlue.setForeground(Qt::darkBlue);

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

//       QTextCharFormat formatCyan = cursor.charFormat();
//       formatCyan.setFontPointSize(fontSize);
//       formatCyan.setForeground(Qt::cyan);

       QTextCharFormat formatRed = cursor.charFormat();
       formatRed.setFontPointSize(fontSize);
       //formatBirthday.setForeground(Qt::darkGreen);
       formatRed.setForeground(Qt::red);


       QTextCharFormat formatSelectedDay = cursor.charFormat();
       formatSelectedDay.setFontPointSize(fontSize);
       formatSelectedDay.setBackground(Qt::green);


       QTextCharFormat boldFormat = format;
       boldFormat.setFontWeight(QFont::Bold);

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
               cellCursor.insertText(QString("%1").arg(date.day()), highlightedFormat);

           }
           else if(date==selectedDate){
               cellCursor.insertText(QString("%1").arg(date.day()), formatSelectedDay);
           }
           else
               cellCursor.insertText(QString("%1").arg(date.day()), format);

           //-----------------------------------------------------
           // Add UK holidays to calendar
           //-----------------------------------------------------
           if(flagShowHolidays){

               foreach (Holiday h, holidayList)
               {
                   QDate d =QDate::fromString(h.m_date);

                   if(date ==d)
                   {
                       cellCursor.insertText("\n"+h.m_name, formatDarkMagenta);
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
                       cellCursor.insertText("\nBirthday: "+name, formatDarkCyan);
                   }
               }
           }


           //-----------------------------------------------------
           //Add appointments to calendar
           //-----------------------------------------------------
           foreach(Appointment a, appointmentList)
           {
               QDate d =QDate::fromString(a.m_date);
               if(date ==d)
               {
                   if(a.m_category=="Event")
                   {
                       //cellCursor.insertText(QString("%1").arg(date.day()), format);
                       cellCursor.insertText("\n"+a.m_title, formatBlue);
                   }
                   else if(a.m_category=="Family"){
                       cellCursor.insertText("\n"+a.m_title, formatDarkYellow);
                   }
                   else if(a.m_category=="Leisure"){
                       cellCursor.insertText("\n"+a.m_title, formatDarkYellow);
                   }

                   else if(a.m_category=="Meeting"){
                       cellCursor.insertText("\n"+a.m_title, formatBlue);
                   }
                   else if(a.m_category=="Work"){
                       cellCursor.insertText("\n"+a.m_title, formatBlue);
                   }
                   else if(a.m_category=="Vacation"){
                       cellCursor.insertText("\n"+a.m_title, formatDarkGreen);
                   }

                   else if(a.m_category=="Medical"){
                       cellCursor.insertText("\n"+a.m_title, formatRed);
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
    h1.m_name="Christmas";
    h1.m_date=QDate(year,12,25).toString();
    h1.m_addToCalendar=1;
    holidayList.append(h1);

    Holiday h2;
    h2.m_id=2;
    h2.m_name="Boxing Day";
    h2.m_date=QDate(year,12,26).toString();
    h2.m_addToCalendar=1;
    holidayList.append(h2);

    Holiday h3;
    h3.m_id=3;
    h3.m_name="New Years Day";
    h3.m_date=QDate(year,1,1).toString();
    h3.m_addToCalendar=1;
    holidayList.append(h3);

    QDate easterSunday =CalculateEaster(year);

    Holiday h4;
    h4.m_id=4;
    h4.m_name="Easter";
    h4.m_date=easterSunday.toString();
    h4.m_addToCalendar=1;
    holidayList.append(h4);

    Holiday h5;
    h5.m_id=5;
    h5.m_name="Good Friday";
    h5.m_date=easterSunday.addDays(-2).toString();
    h5.m_addToCalendar=1;
    holidayList.append(h5);

    Holiday h6;
    h6.m_id=6;
    h6.m_name="Easter Monday";
    h6.m_date=easterSunday.addDays(1).toString();
    h6.m_addToCalendar=1;
    holidayList.append(h6);
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
    fontSize =fontSize+1;
    if (fontSize >24) fontSize =24;
    UpdateCalendar();
}

void MainWindow::on_actionDecrease_Font_triggered()
{
    fontSize=fontSize-1;
    if (fontSize<8) fontSize=8;
    UpdateCalendar();
}

void MainWindow::on_actionReset_Calendar_Font_Size_triggered()
{
    fontSize =12;
    UpdateCalendar();
}

void MainWindow::on_actionUpcoming_Schedule_triggered()
{
    checkAppointmentsNextSevenDays();
}

void MainWindow::on_actionToday_triggered()
{
    //fontSize=12;
    gotoToday();
}
