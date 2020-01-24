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

    //Set Contacts TableView
    ui->tableViewContacts->horizontalHeader()->setVisible(true);
    ui->tableViewContacts->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewContacts->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewContacts->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewContacts->verticalHeader()->setVisible(false);


    //Setup widget Calendar
    ui->tableWidgetCalendar->setColumnCount(columnCount);
    ui->tableWidgetCalendar->setRowCount(rowCount);
    ui->tableWidgetCalendar->horizontalHeader()->setVisible(true);
    ui->tableWidgetCalendar->verticalHeader()->setVisible(false);
    ui->tableWidgetCalendar->setShowGrid(true);
    ui->tableWidgetCalendar->setGridStyle(Qt::SolidLine);

    ui->tableWidgetCalendar->horizontalHeader()
            ->setSectionResizeMode(QHeaderView::ResizeToContents);


    //Calendar Resizing

    ui->tableWidgetCalendar->setWordWrap(true);
    ui->tableWidgetCalendar->setTextElideMode(Qt::ElideRight);
    ui->tableWidgetCalendar->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidgetCalendar->setSelectionMode(QAbstractItemView::NoSelection);

    ui->tableWidgetCalendar->horizontalHeader()->setMinimumSectionSize(120);

    ui->tableWidgetCalendar->verticalHeader()->setMinimumSectionSize(60);


    themeType=1;
    fontSize=12;
    SetTheme(themeType,fontSize);


    for (int row=0; row<ui->tableWidgetCalendar->rowCount(); ++row)
    {
        //ui->tableWidgetCalendar->verticalHeader()->setSectionResizeMode(row, QHeaderView::Stretch);
        ui->tableWidgetCalendar->verticalHeader()
                ->setSectionResizeMode(row, QHeaderView::ResizeToContents);
    }

    for(int col=0; col<ui->tableWidgetCalendar->columnCount(); ++col)
    {
        ui->tableWidgetCalendar->horizontalHeader()->
                setSectionResizeMode(col, QHeaderView::Stretch);

        //ui->tableWidgetCalendar->horizontalHeader()->setSizeAdjustPolicy(QHeaderView::AdjustToContents);

        //ui->tableWidgetCalendar->setItemDelegateForColumn(col, &elideLeftItem);
    }

    firstDay = Qt::Monday;

    ui->labelMonthYear->setText
            (tr("%1 %2"
                ).arg(QLocale::system().monthName(selectedDate.month())
                      ).arg(selectedDate.year()));


    QFont font2 = ui->labelMonthYear->font();
    font2.setPointSize(fontSize+2);
    font2.setBold(true);
    ui->labelMonthYear->setFont(font2);


    ui->actionShow_Birthdays->setChecked(true);
    flagShowBirthdays=true;

    ui->actionShow_Holidays->setChecked(true);
    flagShowHolidays=true;

    ui->actionShow_General->setChecked(true);
    ui->actionShow_Family->setChecked(true);
    ui->actionShow_Leisure->setChecked(true);
    ui->actionShow_Meetings->setChecked(true);
    ui->actionShow_Work->setChecked(true);
    ui->actionShow_Vacations->setChecked(true);
    ui->actionShow_Medical->setChecked(true);
    ui->actionShow_Fitness->setChecked(true);


    //Keyboard QActions (shortcuts)
    //----------------------------
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

    newContactAction=new QAction(this);
    newContactAction->setShortcut(Qt::Key_C);
    connect(newContactAction, SIGNAL(triggered()),
            this, SLOT(newContactSlot()));
    this->addAction(newContactAction);

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
    //------------------------------------------

    //Setup empty lists
    appointmentList= QList<Appointment>();
    holidayList=QList<Holiday>();
    contactList =QList<Contact>();
    //set up models
    dayListModel= new DayListModel;
    contactModel =new ContactModel;
    proxyModelContacts = new ProxyModelContacts;

    //AddHolidaysToHolidayList(selectedYear);

    //Add contacts to contactList
    LoadDatebaseContactsToContactList(); //loads contactlist
    DisplayContactsOnTableView();
    AddHolidaysToHolidayList(selectedYear);
    LoadDatabaseAppointmentsToAppointmentList();
    UpdateCalendar();
    ShowAppointmentsOnListView(selectedDate);

    //Translations (to do..)
    ui->menuFile->setTitle(tr("&File"));
    //ui->menuFile->setToolTip(tr("File"));
    ui->actionExport_Contacts->setText(tr("Export Contacts XML"));
    ui->actionExport_Contacts->setToolTip(tr("Export Contacts XML"));
    ui->actionImport_Contacts->setText(tr("Import Contacts XML"));
    ui->actionImport_Contacts->setToolTip(tr("Import Contacts XML"));

    ui->actionExport_Appointments->setText(tr("Export Appointments XML"));
    ui->actionExport_Appointments->setToolTip(tr("Export Appointments XML"));
    ui->actionImport_Appointments->setText(tr("Import Appointments XML"));
    ui->actionImport_Appointments->setToolTip(tr("Import Appointments XML"));


    ui->actionExit->setText(tr("Exit"));
    ui->actionExit->setToolTip(tr("Exit"));

    //Edit menu
    ui->menuEdit->setTitle(tr("&Edit"));
    //ui->menuEdit->setToolTip(tr("Edit"));
    ui->actionDelete_All_Appointments->setText(tr("Delete All Appointments"));
    ui->actionDelete_All_Appointments->setToolTip(tr("Delete All Appointments"));
    ui->actionDelete_All_Contacts->setText(tr("Delete All Contacts"));
    ui->actionDelete_All_Contacts->setToolTip(tr("Delete All Contacts"));

    //Appointments
    ui->menuAppointments->setTitle(tr("&Appointments"));
    ui->menuAppointments->setToolTip(tr("Appointments"));

    ui->actionNew_Appointment->setText(tr("New Appointment"));
    ui->actionNew_Appointment->setToolTip(tr("New Appointment"));
    ui->actionGeneerate_Repeat_Appointments->setText(tr("Generate Repeat Appointments"));
    ui->actionGeneerate_Repeat_Appointments->setToolTip(tr("Generate Repeat Appointments"));
    ui->actionUpcoming_Schedule->setText(tr("Upcoming Schedule"));
    ui->actionUpcoming_Schedule->setToolTip(tr("Upcoming Schedule"));

    //Contacts
    ui->menuContacts->setTitle(tr("&Contacts"));
    ui->menuContacts->setToolTip(tr("Contacts"));
    ui->actionNew_Contact->setText(tr("New Contact"));
    ui->actionNew_Contact->setToolTip(tr("New Contact"));
    ui->actionCheck_For_Birthdays->setText(tr("Check For Birthdays"));
    ui->actionCheck_For_Birthdays->setToolTip(tr("Check For Birthdays"));

    ui->menuNavigation->setTitle(tr("&Navigation"));
    ui->menuNavigation->setToolTip(tr("Navigation"));
    ui->actionNext_Month->setText(tr("Next Month"));
    ui->actionNext_Month->setToolTip(tr("Next Month"));
    ui->actionPrevious_Month->setText(tr("Previous Month"));
    ui->actionPrevious_Month->setToolTip(tr("Previous Month"));
    ui->actionToday->setText(tr("Today (Spacebar)"));
    ui->actionToday->setToolTip(tr("Today"));

    //Themes (more to do..)
    ui->menuThemes->setTitle(tr("&Themes"));
    ui->menuThemes->setToolTip(tr("Calendar Themes"));

    //Options menu
    ui->menuOptions->setTitle(tr("&Options"));
    ui->menuOptions->setToolTip(tr("Options"));

    ui->actionShow_Birthdays->setText(tr("Show Birthdays On Calendar"));
    ui->actionShow_Birthdays->setToolTip(tr("Show Birthdays On Calendar"));

    ui->actionShow_Holidays->setText(tr("Show Holidays On Calendar"));
    ui->actionShow_Holidays->setToolTip(tr("Show Holidays On Calendar"));


    ui->actionShow_General->setText(tr("Show General"));
    ui->actionShow_General->setToolTip(tr("Show General"));

    ui->actionShow_Family->setText(tr("Show Family"));
    ui->actionShow_Family->setToolTip(tr("Show Family"));

    ui->actionShow_Leisure->setText(tr("Show Leisure"));
    ui->actionShow_Leisure->setToolTip(tr("Show Leisure"));

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

    //Help
    ui->menuHelp->setTitle(tr("&Help"));
    ui->actionAbout->setText(tr("&About"));
    ui->actionAbout->setToolTip(tr("About"));

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
            a.m_id =appointmentId;

            appointmentList.append(a);
            //dayListModel->addAppointment(a);
            //LoadDatabaseAppointmentsToAppointmentList(); //sync

            UpdateCalendar();
            ShowAppointmentsOnListView(selectedDate);
        }
    }
 UpdateCalendar();
}

void MainWindow::UpdateAppointment(int dbID, int selectedRowindex)
{
    Appointment currentAppointment = dbm.getAppointmentByID(dbID);

        DialogAppointmentUpdate *appointmentDialog =
                new  DialogAppointmentUpdate(this,&currentAppointment);
        appointmentDialog->setModal(true);


        if (appointmentDialog->exec() == QDialog::Accepted ) {
            //qDebug()<<"Delete request = "<<appointmentDialog->getDeleteRequested();

            if(appointmentDialog->getDeleteRequested())
            {

                RemoveAppointmentFromAppointmentList(dbID);
                dbm.deleteAppointmentById(dbID);
                //update reminder listview
                dayListModel->removeAppointment(selectedRowindex);
                // may have to remove reminder from modellist

                ui->listViewDay->setModel(dayListModel);
                UpdateCalendar();
                return;
            }

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
                dbm.updateAppointment(a,dbID);
                //bool success =dbm.updateAppointment(a,dbID);
                //qDebug()<<"Apointment update: success ="<<success;

                LoadDatabaseAppointmentsToAppointmentList(); //sync with db
                UpdateCalendar();
                ShowAppointmentsOnListView(selectedDate);
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
                    tmp.m_id=appointmentId;
                    appointmentList.append(tmp);

                    UpdateCalendar();
                    ShowAppointmentsOnListView(selectedDate);
                    //qDebug()<<"Added Appointment: Appointment ID = "<<appointmentId;
                }
            }
            UpdateCalendar();
        }

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

void MainWindow::ShowAppointmentsOnListView(QDate theSelectedDate)
{
    dayListModel->clearAllAppointment();
    QList<Appointment> sortedDayList =getSortedDayList(theSelectedDate);

    QList<Appointment> sortedDayList2= QList<Appointment>();

    if (!sortedDayList.empty())
    {
        foreach(Appointment a, sortedDayList)
        {
            if(a.m_category=="General" && flagShowGeneralEvents)
            {
                sortedDayList2.append(a);
            }
            else if(a.m_category=="Meeting" && flagShowMeetings){
                sortedDayList2.append(a);
            }
            else if(a.m_category=="Work" && flagShowWorkEvents){
                sortedDayList2.append(a);
            }
            else if(a.m_category=="Family" && flagShowFamilyEvents){

                sortedDayList2.append(a);
            }
            else if(a.m_category=="Leisure" && flagShowLeisureEvents){
                sortedDayList2.append(a);

            }
            else if(a.m_category=="Fitness" && flagShowFitness){
                sortedDayList2.append(a);
            }


            else if(a.m_category=="Vacation" && flagShowVacations){
                sortedDayList2.append(a);
            }

            else if(a.m_category=="Medical" && flagShowMedical){
                sortedDayList2.append(a);
            }
        }
    }


    dayListModel = new DayListModel(sortedDayList2);
    dayListModel->setThemeType(themeType);
    ui->listViewDay->setModel(dayListModel);
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

QList<Appointment> MainWindow::getSortedDayList(QDate theDate)
{
    QList<Appointment> dayList =QList<Appointment>();
        foreach(Appointment a, appointmentList)
        {
            QDate adate = QDate::fromString(a.m_date);

            if(adate==theDate)
            {
                dayList.append(a);
            }
        }
        std::sort(dayList.begin(), dayList.end(), compare);

        return dayList;

}

void MainWindow::UpdateCalendar()
{
    int cellIndex=0;
    ui->tableWidgetCalendar->clearContents();
    //Initialise with empty cells
    for (int row=0; row<ui->tableWidgetCalendar->rowCount(); ++row)
    {
        for(int col=0; col<ui->tableWidgetCalendar->columnCount(); ++col)
        {
            ui->tableWidgetCalendar->setItem(row, col,
                                     new QTableWidgetItem(""));
            cellIndex = (7 * row) + col;
            dayArray[cellIndex]=0;
        }
    }

    QStringList days;
    for (int weekDay = 1; weekDay <= 7; ++weekDay) {
        QString day =QLocale::system().dayName(weekDay); //boldFormat
        days.append(day);
    }
    ui->tableWidgetCalendar->setHorizontalHeaderLabels(days);

    ui->tableWidgetCalendar->setHorizontalHeaderLabels(days);

    QDate date(selectedDate.year(), selectedDate.month(), 1);

    int row=0;
    int dayValue=0;
    while (date.month() == selectedDate.month()) {
        int weekDay = date.dayOfWeek();       
        dayValue= date.day();       
        cellIndex = (7 * row) + weekDay-1;       
        dayArray[cellIndex]=dayValue;
        dayItem = new QTableWidgetItem(QString::number(dayValue));

        ui->tableWidgetCalendar->setItem(row, weekDay-1,dayItem);

//        if (date==QDate::currentDate())        {

//
//                dayItem->setBackgroundColor(Qt::yellow);
//

//        }

        QString str="";
        //-----------------------------------------------------
        // Add holidays to calendar
        //-----------------------------------------------------
        if(flagShowHolidays){

            foreach (Holiday h, holidayList)
            {
                QDate d =QDate::fromString(h.m_date);

                if(date ==d)
                {
                    //add holiday to calendar
                    str.append("\n"+h.m_name);

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
                    str.append("\nBirthday: "+name);
                }
            }
        }

        //---------------------------------------------------------
        // Add Appointments
        //----------------------------------------------------------

        QList<Appointment> sortedDayList =getSortedDayList(date);

        if (!sortedDayList.empty())
        {
            foreach(Appointment a, sortedDayList)
            {
                if(a.m_category=="General" && flagShowGeneralEvents)
                {
                    str.append("\n"+a.m_title);
                }
                else if(a.m_category=="Meeting" && flagShowMeetings){
                    str.append("\n"+a.m_title);
                }
                else if(a.m_category=="Work" && flagShowWorkEvents){
                    str.append("\n"+a.m_title);
                }
                else if(a.m_category=="Family" && flagShowFamilyEvents){

                    str.append("\n"+a.m_title);
                }
                else if(a.m_category=="Leisure" && flagShowLeisureEvents){
                    str.append("\n"+a.m_title);

                }
                else if(a.m_category=="Fitness" && flagShowFitness){
                    str.append("\n"+a.m_title);
                }


                else if(a.m_category=="Vacation" && flagShowVacations){
                    str.append("\n"+a.m_title);
                }

                else if(a.m_category=="Medical" && flagShowMedical){
                    str.append("\n"+a.m_title);
                }                

            }
            //Display on calendar
        }

           calendarItem = new QTableWidgetItem(QString::number(dayValue)
                                                  +str);

           calendarItem->setTextColor(Qt::black);
           ui->tableWidgetCalendar->setItem(row,weekDay-1, calendarItem);

           if (date==QDate::currentDate())
           {

               ui->tableWidgetCalendar->item(row, weekDay-1)->setData(
                           Qt::BackgroundRole,
                           QBrush(Qt::lightGray)
                           );

           }


            //Move on ...
            date = date.addDays(1); //move to next day
        if (weekDay == 7 && date.month() == selectedDate.month())
        {
            row=row+1;
        }

    }//selected month


    ui->labelMonthYear->setText
              (tr("%1 %2"
                  ).arg(QLocale::system().monthName(selectedDate.month())
                        ).arg(selectedDate.year()));
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

    ShowAppointmentsOnListView(selectedDate);
    selectedDateLabel->setText(selectedDate.toString());
    UpdateCalendar();
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
    ShowAppointmentsOnListView(selectedDate);
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
    ShowAppointmentsOnListView(selectedDate);
    selectedDateLabel->setText(selectedDate.toString());
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

    QDate firstMondayMay(year, 5, 1);

    while (firstMondayMay.dayOfWeek() != Qt::Monday)
            firstMondayMay = firstMondayMay.addDays(1);

    h7.m_date=firstMondayMay.toString();
    h7.m_addToCalendar=1;
    holidayList.append(h7);

    //Days in May

    int daysInMay = QDate(year,5,1).daysInMonth();
    int plusDays=0;

    if (firstMondayMay.day()+28<=daysInMay){

        plusDays =28;
    }
    else {
        plusDays=21;
    }


    Holiday h8;
    h8.m_id=8;
    h8.m_name=tr("Spring Bank Holiday");
    QDate springbank =firstMondayMay.addDays(plusDays);

    if (springbank.isValid())
    {
        h8.m_date=springbank.toString();
        h8.m_addToCalendar=1;
        holidayList.append(h8);
    }   

    Holiday h9;
    h9.m_id=9;
    h9.m_name=tr("Summer Bank Holiday");

    QDate firstMondayAug(year, 8, 1);

    while (firstMondayAug.dayOfWeek() != Qt::Monday)
            firstMondayAug = firstMondayAug.addDays(1);

    int daysInAugust = QDate(year,8,1).daysInMonth();
    plusDays=0;

    if (firstMondayAug.day()+28<=daysInAugust){

        plusDays =28;
    }
    else {
        plusDays=21;
    }

    QDate summerBankHol =firstMondayAug.addDays(plusDays);

    if(summerBankHol.isValid())
    {
        h9.m_date=summerBankHol.toString();
        h9.m_addToCalendar=1;
        holidayList.append(h9);
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

void MainWindow::newContactSlot()
{
    NewContact();
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



void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_tableWidgetCalendar_cellClicked(int row, int column)
{

    int day =dayArray[(7 * row) + column];
    //check if dayVal>0

    if((day<1) || (day>selectedDate.daysInMonth()) ) return;

    selectedDate =QDate(selectedYear,selectedMonth,day);
    //qDebug()<<"Selected Date = "<<theDate.toString();

    selectedDateLabel->setText(selectedDate.toString());
    ShowAppointmentsOnListView(selectedDate);


}

void MainWindow::on_actionNew_Appointment_triggered()
{
    NewAppointment();
}

void MainWindow::on_listViewDay_doubleClicked(const QModelIndex &index)
{
    int selectedRowIdx=index.row();
    Appointment tmp =dayListModel->getAppointment(selectedRowIdx);
    int dbId =tmp.m_id;
    qDebug()<<"dbID = "<<dbId;
    UpdateAppointment(dbId,selectedRowIdx);
}

void MainWindow::on_tableWidgetCalendar_cellDoubleClicked(int row, int column)
{

    int day =dayArray[(7 * row) + column];
    if((day<1) || (day>selectedDate.daysInMonth()) ) return;
    selectedDate =QDate(selectedYear,selectedMonth,day);
    //qDebug()<<"Selected Date = "<<theDate.toString();
    selectedDateLabel->setText(selectedDate.toString());
    ShowAppointmentsOnListView(selectedDate);
    NewAppointment();
}

void MainWindow::on_actionGeneerate_Repeat_Appointments_triggered()
{
    GenerateRepeatAppointments();
}

void MainWindow::on_actionUpcoming_Schedule_triggered()
{
    checkAppointmentsNextSevenDays();
}

void MainWindow::on_actionNew_Contact_triggered()
{
    NewContact();
}

void MainWindow::on_actionDelete_All_Appointments_triggered()
{
    dbm.deleteAllAppointments();
    appointmentList.clear();
    UpdateCalendar();
    ShowAppointmentsOnListView(selectedDate);
}



void MainWindow::on_actionDelete_All_Contacts_triggered()
{
    dbm.removeAllContacts();
    contactList.clear();
    DisplayContactsOnTableView();
    UpdateCalendar(); //remove birthdays
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout *aboutDialog = new DialogAbout(this);
    aboutDialog->setModal(false);
    aboutDialog->exec();
}

void MainWindow::on_tableViewContacts_clicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data();
    selectedContactDbId =idd.toInt();
    //qDebug()<<"SelectedContactId = "<<selectedCont
    selectedContact=dbm.getContactByID(selectedContactDbId);
}

void MainWindow::on_pushButtonMailTo_clicked()
{
    QString url="mailto:?to="+selectedContact.m_email+"&subject=Enter the subject&body=Enter message";
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
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

void MainWindow::on_tableViewContacts_doubleClicked(const QModelIndex &index)
{
    QVariant idd = index.sibling(index.row(),0).data();
    selectedContactDbId =idd.toInt();
    //qDebug()<<"SelectedContactId = "<<selectedContactDbId;
    UpdateContact(selectedContactDbId);
}

void MainWindow::on_actionNext_Month_triggered()
{
    gotoNextMonth();
}

void MainWindow::on_actionPrevious_Month_triggered()
{
    gotoPreviousMonth();
}

void MainWindow::on_actionToday_triggered()
{
    gotoToday();
}

void MainWindow::on_actionCheck_For_Birthdays_triggered()
{
    checkForBirthdaysNextSevenDays();
}

void MainWindow::on_actionShow_Birthdays_triggered()
{
    if(ui->actionShow_Birthdays->isChecked())

       {
            ui->actionShow_Birthdays->setChecked(true);
            flagShowBirthdays=true;

        }
        else {
            ui->actionShow_Birthdays->setChecked(false);
            flagShowBirthdays=false;
        }
        UpdateCalendar();
}

void MainWindow::on_actionShow_Holidays_triggered()
{
    if(ui->actionShow_Holidays->isChecked())
       {
          ui->actionShow_Holidays->setChecked(true);
          flagShowHolidays=true;
       }
       else {
          ui->actionShow_Holidays->setChecked(false);
          flagShowHolidays=false;
       }

       UpdateCalendar();
       ShowAppointmentsOnListView(selectedDate);
}

void MainWindow::on_actionShow_General_triggered()
{
    if(ui->actionShow_General->isChecked())
    {
        ui->actionShow_General->setChecked(true);
        flagShowGeneralEvents=true;
    }
    else {
        ui->actionShow_General->setChecked(false);
        flagShowGeneralEvents=false;
    }
    UpdateCalendar();
    ShowAppointmentsOnListView(selectedDate);

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
    ShowAppointmentsOnListView(selectedDate);

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
    ShowAppointmentsOnListView(selectedDate);

}

void MainWindow::on_actionShow_Leisure_triggered()
{
    if(ui->actionShow_Leisure->isChecked())
    {
        ui->actionShow_Leisure->setChecked(true);
        flagShowLeisureEvents=true;
    }
    else {
        ui->actionShow_Leisure->setChecked(false);
        flagShowLeisureEvents=false;
    }
    UpdateCalendar();
    ShowAppointmentsOnListView(selectedDate);


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
    ShowAppointmentsOnListView(selectedDate);

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
    ShowAppointmentsOnListView(selectedDate);


}

void MainWindow::on_actionShow_Family_triggered()
{
    if(ui->actionShow_Family->isChecked())
    {
        ui->actionShow_Family->setChecked(true);
        flagShowFamilyEvents=true;
    }
    else {
        ui->actionShow_Family->setChecked(false);
        flagShowFamilyEvents=false;
    }
    UpdateCalendar();
    ShowAppointmentsOnListView(selectedDate);

}

void MainWindow::SetTheme(int type, int fontsize)
{
    if (type==1)
    {

        QString strfont ="font-size: "+ QString::number(fontsize)+"pt;";
        //Light theme
        QString style(
                    "QTableWidget {"
                    "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f4f4f6, stop:1 #ceced6);"
                    "border-width: 2px;"
                    "border-style: solid;"
                    "border-color: black;"
                    " gridline-color: black;"
                    "color: black;"
                    +strfont+
                    "selection-color: darkblue"
                    "}"
                    "QTableWidget::item {"
                    "background: none;"
                    "}"
                    );

        ui->tableWidgetCalendar->setStyleSheet(style);
        QFont font = ui->tableWidgetCalendar->horizontalHeader()->font();
        font.setPointSize(fontsize+1);
        ui->tableWidgetCalendar->horizontalHeader()->setFont(font);
        QFont font3 = ui->listViewDay->font();
        font3.setPointSize(fontsize);
        ui->listViewDay->setFont(font3);
        //ui->listViewDay->setBackgroundRole(QPalette::NoRole);
        ui->listViewDay->setBackgroundRole(QPalette::Window);
        //(QBrush(Qt::lightGray));


    }
    else if (type==2)
    {
        //Dark theme
        QString strfont ="font-size: "+ QString::number(fontsize)+"pt;";
        QString style1(
                    "QTableWidget {"
                    "background-color:#232326;"
                    "border-width: 2px;"
                    "border-style: solid;"
                    "border-color: white;"
                    "gridline-color: white;"
                    "color: white;"
                    +strfont+
                    "}"
                    "QTableWidget::item {"
                    "color:white;"
                    "selection-color: #000000;"
                    "background: none;"
                    "}"
                    "QTableWidget::item:selected {"
                    "color:white;"
                    "background:#000000; "
                    "font-weight:900;"
                    "}"
                    );


        ui->tableWidgetCalendar->setStyleSheet(style1);
        QFont font = ui->tableWidgetCalendar->horizontalHeader()->font();
        font.setPointSize(fontsize+2);
        ui->tableWidgetCalendar->horizontalHeader()->setFont(font);
        QFont font3 = ui->listViewDay->font();
        font3.setPointSize(fontsize+1);
        ui->listViewDay->setFont(font3);
        ui->listViewDay->setBackgroundRole(QPalette::Window);
    }

    else if (type ==3)
    {
        //Garish green
        QString strfont ="font-size: "+ QString::number(fontsize)+"pt;";
        QString styleGarish(
                    "QTableWidget {"
                    "background-color: qradialgradient(cx:0, cy:0, radius: 1, fx:0.5, fy:0.5, stop:0 white, stop:1 green);"
                    "border-width: 2px;"
                    "border-style: solid;"
                    "border-color: red;"
                    " gridline-color: red;"
                    +strfont+
                    "selection-color: darkblue"
                    "}"
                    "QTableWidget::item {"
                    "background: none;"
                    "}"
                    );

        ui->tableWidgetCalendar->setStyleSheet(styleGarish);
        QFont font = ui->tableWidgetCalendar->horizontalHeader()->font();
        font.setPointSize(fontsize+1);
        ui->tableWidgetCalendar->horizontalHeader()->setFont(font);
        QFont font3 = ui->listViewDay->font();
        font3.setPointSize(fontsize);
        ui->listViewDay->setFont(font3);
        ui->listViewDay->setBackgroundRole(QPalette::Window);
    }
    else if (type ==4)
    {
        //Garish blue
        QString strfont ="font-size: "+ QString::number(fontsize)+"pt;";
        QString styleGarishBlue(
                    "QTableWidget {"
                    "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 white, stop:1 blue);"
                    "border-width: 2px;"
                    "border-style: solid;"
                    "border-color: red;"
                    " gridline-color: red;"
                    +strfont+
                    "selection-color: darkblue"
                    "}"
                    "QTableWidget::item {"
                    "background: none;"
                    "}"
                    );

        ui->tableWidgetCalendar->setStyleSheet(styleGarishBlue);
        QFont font = ui->tableWidgetCalendar->horizontalHeader()->font();
        font.setPointSize(fontsize+1);
        ui->tableWidgetCalendar->horizontalHeader()->setFont(font);
        QFont font3 = ui->listViewDay->font();
        font3.setPointSize(fontSize);
        ui->listViewDay->setFont(font3);
        ui->listViewDay->setBackgroundRole(QPalette::Window);
    }
}

void MainWindow::increaseFont()
{
    fontSize=fontSize+1;
    if (fontSize>=36) fontSize=36;
    SetTheme(themeType,fontSize);
}

void MainWindow::decreaseFont()
{
    fontSize=fontSize-1;
    if (fontSize<=8) fontSize=8;
    SetTheme(themeType,fontSize);
}

void MainWindow::resetFont()
{
    fontSize=12;
    SetTheme(themeType,fontSize);
}

void MainWindow::on_actionStandard_Theme_triggered()
{
    themeType=1;
    SetTheme(1,fontSize);

}

void MainWindow::on_actionGarish_Theme_triggered()
{
    themeType=3;
    SetTheme(themeType,13);
}

void MainWindow::on_actionGarish_Blue_triggered()
{

    themeType=4;
    SetTheme(themeType,13);
}

void MainWindow::on_actionExport_Appointments_triggered()
{
    ExportAppointmentsXML();
}

void MainWindow::on_actionImport_Appointments_triggered()
{
    ImportAppointmentsXML();
}

void MainWindow::on_actionExport_Contacts_triggered()
{
    ExportContactsXML();
}

void MainWindow::on_actionImport_Contacts_triggered()
{
    ImportContactsXML();
}

void MainWindow::on_actionIncrease_Font_triggered()
{
    increaseFont();
}

void MainWindow::on_actionDecrease_Font_triggered()
{
    decreaseFont();
}

void MainWindow::on_actionReset_Font_triggered()
{
    resetFont();
}

void MainWindow::on_actionDark_Theme_triggered()
{
    themeType=2;
    SetTheme(themeType, fontSize);
}
