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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QtWidgets>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QDomDocument>

#include "appointment.h"
#include "contact.h"
#include "holiday.h"

#include "dbmanager.h"
#include "dialogshowdaydetails.h"
#include "dialogappointment.h"
#include "dialogcontact.h"
#include "contactmodel.h"
#include "proxymodelcontacts.h"
#include "dialogabout.h"
#include "dialogupcomingschedule.h"
#include "dialogupcomingbirthdays.h"
#include "dialogrepeatappointment.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    DbManager dbm;


    int appointmentId =0;
    int contactId=0;
    int parentId=1;
    Appointment parentAppointment;


    //Appointments
    QString title="";
    QString location="";
    QString description="";
    QDate appointmentDate;
    QTime appointmentStartTime;
    QTime appointmentEndTime;
    QString category="";    
    int isAllDay=0;

    int isRepeating=0;
    int repeatDayInterval=0;
    int repeatNumber=0;

    int selectedAppointmentdDbId =0;

    //Appointments
    void NewAppointment();
    void GenerateRepeatAppointments();
    static bool compare(const Appointment& first, const Appointment& second);
    //AppointmentList methods
    void LoadDatabaseAppointmentsToAppointmentList();

    //Contacts
    QString contactFirstName ="";
    QString contactMiddleNames="";
    QString contactLastName="";
    QString contactEmail="";
    QString street="";
    QString district="";
    QString city="";
    QString county="";
    QString postcode ="";
    QString country ="";
    QString phoneNumber="";
    QDate birthDate=QDate(); //(0,0,0)
    int addBirthdayToCalendar =0;
    int selectedContactDbId=0;
    Contact selectedContact;

    ContactModel *contactModel;
    ProxyModelContacts *proxyModelContacts;


    void LoadDatebaseContactsToContactList();
    void NewContact();
    void UpdateContact(int dbID);
    void DisplayContactsOnTableView();

    //ContactList methods
    void RemoveContactFromContactList(int contactId);


    QAction *newAppointmentAction;
    QAction *newContactAction;

    //Flags
    bool flagShowBirthdays=true;
    bool flagShowHolidays =true;
    bool flagShowGeneralEvents=true;
    bool flagShowFamilyEvents =true;
    bool flagShowLeisureEvents =true;
    bool flagShowMeetings=true;
    bool flagShowWorkEvents =true;
    bool flagShowVacations=true;
    bool flagShowMedical=true;
    bool flagColourCoding=true;
    bool flagShowFitness=true;



    bool flagQuickView=false;


    //Helper methods
     QDate CalculateEaster(int year);
     void checkForBirthdaysNextSevenDays();
     void checkAppointmentsNextSevenDays();

     //Font
     void increaseFont();
     void decreaseFont();
     void resetFont();

     QAction *increaseFontAction;
     QAction *decreaseFontAction;
     QAction *resetFontAction;


     //Navigation
     void gotoNextDay();
     void gotoPreviousDay();
     void gotoNextMonth();
     void gotoPreviousMonth();
     void gotoToday();
     void showDayEvents();

     QAction *gotoNextDayAction;
     QAction *gotoPreviousDayAction;
     QAction *gotoNextMonthAction;
     QAction *gotoPreviousMonthAction;
     QAction *gotoTodayAction;
     QAction *showAppointmentDetailsAction;

     QLabel *selectedDateLabel;
     QLabel *dateInfoLabel;

    //Export Import
    void ExportContactsXML();
    void ImportContactsXML();
    void ExportAppointmentsXML();
    void ImportAppointmentsXML();


private slots:

    void gotoNextDaySlot();
    void gotoPreviousDaySlot();
    void gotoNextMonthSlot();
    void gotoPreviousMonthSlot();
    void gotoTodaySlot();
    void showAppointmentDetailsSlot();

    void increaseFontSlot();
    void decreaseFontSlot();
    void resetFontSlot();

    void newAppointmentSlot();
    void newContactSlot();

    void on_actionExit_triggered();

    void on_actionNext_Day_triggered();

    void on_actionPrevious_Day_triggered();

    void on_actionNext_Month_triggered();

    void on_actionPrevious_Month_triggered();

    void on_actionNew_Appointment_triggered();

    void on_actionNew_Contact_triggered();

    void on_tableViewContacts_doubleClicked(const QModelIndex &index);

    void on_actionCheck_For_Birthdays_triggered();

    void on_pushButtonShowQuickFullView_clicked();

    void on_pushButtonMailTo_clicked();

    void on_tableViewContacts_clicked(const QModelIndex &index);

    void on_actionShow_Birthdays_on_Calendar_triggered();

    void on_actionShow_Holidays_on_Calendar_triggered();

    void on_actionExport_Contacts_XML_triggered();

    void on_actionImport_Contacts_XML_triggered();

    void on_actionDelete_All_Appointments_triggered();

    void on_actionDelete_All_Contacts_triggered();

    void on_actionAbout_triggered();

    void on_actionShow_Day_Events_triggered();

    void on_action_Increase_Font_triggered();

    void on_actionDecrease_Font_triggered();

    void on_actionReset_Calendar_Font_Size_triggered();

    void on_actionUpcoming_Schedule_triggered();

    void on_actionToday_triggered();

    void on_actionGenerate_Repeat_Appointments_triggered();

    void on_actionShow_General_Events_triggered();

    void on_actionShow_Family_Events_triggered();

    void on_actionShow_Leisure_Events_triggered();

    void on_actionShow_Meetings_triggered();

    void on_actionShow_Work_triggered();

    void on_actionShow_Vacations_triggered();

    void on_actionShow_Medical_triggered();

    void on_actionColour_Code_Appointments_triggered();

    void on_actionShow_Fitness_triggered();

    void on_actionExport_Appointments_XML_triggered();

    void on_actionImport_Appointments_XML_triggered();

private:
    Ui::MainWindow *ui;

    int fontSize;
    QDate selectedDate;
    void UpdateCalendar();

    const char *monthNames[12]= {"January", "February", "March", "April",
                                 "May", "June", "July", "August", "September",
                                 "October", "November", "December"};
    int selectedYear=0;
    int selectedMonth=0;
    int selectedDay=0;

    QList<Appointment> appointmentList;    
    QList<Holiday> holidayList;
    QList <Contact> contactList;

    void AddHolidaysToHolidayList(int year);

};

#endif // MAINWINDOW_H
