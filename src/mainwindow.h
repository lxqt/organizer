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
#include <QMessageBox>
#include <QTextEdit>
#include <QDebug>
#include <QDate>
#include <QTimer>
#include <QLabel>
#include <QtWidgets>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QTableWidgetItem>
#include <QtMultimedia>
#include <QSettings>


#include "appointment.h"
#include "contact.h"
#include "holiday.h"

#include "dbmanager.h"

#include "dialogappointment.h"
#include "dialogappointmentupdate.h"
#include "dialogcontact.h"
#include "contactmodel.h"
#include "proxymodelcontacts.h"
#include "dialogabout.h"
#include "dialogupcomingschedule.h"
#include "dialogupcomingbirthdays.h"
#include "dialogrepeatappointment.h"
#include "daylistmodel.h"
#include "dialogpreferences.h"
#include "preferences.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Database
    DbManager dbm;

    //Timers
    QTimer *timer;
    //QTimer *timerSingleShot;
    //Reminders
    void checkForReminders();

    //Sounds
    QMediaPlayer *player;

    //Preferences default

    int playAudio=1;
    QString locale="English";
    int calendarTheme=0;
    int applicationFontSize=20;
    int calendarFontSize=14;
    Preferences currentPreferences;
    void SetPreferences();
    void SaveSettings();
    bool LoadSettings();

    //Appointments
    QString title="";
    QString location="";
    QString description="";
    QDate appointmentDate;
    QTime appointmentStartTime;
    QTime appointmentEndTime;
    QString category="";
    int isAllDay=0;
    int appointmentId =0;
    int hasReminder=0;
    int reminderMinutes=0;

    int isRepeating=0;
    int repeatDayInterval=0;
    int repeatNumber=0;

    QList<Appointment> appointmentList;

    void NewAppointment();
    void UpdateAppointment(int dbID, int selectedRowindex);
    void GenerateRepeatAppointments();
    void LoadDatabaseAppointmentsToAppointmentList();
    void UpdateAppointmentInAppointmentList(Appointment app, int appointmentId);
    void RemoveAppointmentFromAppointmentList(int appointmentId);

    void ShowAppointmentsOnListView(QDate theSelectedDate);

    //Sorting
    static bool compare(const Appointment& first, const Appointment& second);
    QList<Appointment> getSortedDayList(QDate theDate);

    //Models
    DayListModel *dayListModel;

    //Selected date
     QDate selectedDate;
     int selectedYear=2000;
     int selectedMonth=1;
     int selectedDay=1;
     int firstDay=1;
     QLabel *selectedDateLabel;
     QLabel *dateInfoLabel;


    //Widget Calendar
    int columnCount=7;
    int rowCount=6;
    //int dayArray[6][7];
    int dayArray[6*7];
    void UpdateCalendar();

    void increaseFont();
    void decreaseFont();
    void resetFont();

    QAction *increaseFontAction;
    QAction *decreaseFontAction;
    QAction *resetFontAction;

    QTableWidgetItem* dayItem;
    QTableWidgetItem* calendarItem;
    QTableWidgetItem* holidayItem;
    QTableWidgetItem* birthdayItem;

    //Themes
    void SetTheme();
    int newLineSpacing=0;


    //Helper methods
    QDate CalculateEaster(int year);
    void checkForBirthdaysNextSevenDays();
    void checkAppointmentsNextSevenDays();


    //Navigation   
    void gotoNextMonth();
    void gotoPreviousMonth();
    void gotoToday();
    //void showDayEvents();

    //Keyboard navigation actions
    QAction *gotoNextMonthAction;
    QAction *gotoPreviousMonthAction;
    QAction *gotoTodayAction;


    QList<Holiday> holidayList;
    void AddHolidaysToHolidayList(int year);

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
     int contactId=0;

    ContactModel *contactModel;
    ProxyModelContacts *proxyModelContacts;

    QAction *newContactAction;

    void LoadDatebaseContactsToContactList();
    void NewContact();
    void UpdateContact(int dbID);
    void DisplayContactsOnTableView();

    //ContactList methods
    void RemoveContactFromContactList(int contactId);

    QList <Contact> contactList;


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


    //Export Import
    void ExportContactsXML();
    void ImportContactsXML();
    void ExportAppointmentsXML();
    void ImportAppointmentsXML();



private slots:

    void timerUpdateSlot();

    void gotoNextMonthSlot();
    void gotoPreviousMonthSlot();
    void gotoTodaySlot();
    //void newAppointmentSlot();
    void newContactSlot();

    void increaseFontSlot();
    void decreaseFontSlot();
    void resetFontSlot();

    void on_actionExit_triggered();

    void on_tableWidgetCalendar_cellClicked(int row, int column);

    void on_actionNew_Appointment_triggered();

    void on_listViewDay_doubleClicked(const QModelIndex &index);

    void on_tableWidgetCalendar_cellDoubleClicked(int row, int column);

    void on_actionGeneerate_Repeat_Appointments_triggered();

    void on_actionUpcoming_Schedule_triggered();

    void on_actionNew_Contact_triggered();

    void on_actionDelete_All_Appointments_triggered();

    void on_actionDelete_All_Contacts_triggered();

    void on_actionAbout_triggered();

    void on_tableViewContacts_clicked(const QModelIndex &index);

    void on_pushButtonMailTo_clicked();

    void on_pushButtonShowQuickFullView_clicked();

    void on_tableViewContacts_doubleClicked(const QModelIndex &index);

    void on_actionNext_Month_triggered();

    void on_actionPrevious_Month_triggered();

    void on_actionToday_triggered();

    void on_actionCheck_For_Birthdays_triggered();

    void on_actionShow_Birthdays_triggered();

    void on_actionShow_Holidays_triggered();

    void on_actionShow_General_triggered();

    void on_actionShow_Meetings_triggered();

    void on_actionShow_Work_triggered();

    void on_actionShow_Leisure_triggered();

    void on_actionShow_Fitness_triggered();

    void on_actionShow_Vacations_triggered();

    void on_actionShow_Medical_triggered();

    void on_actionShow_Family_triggered();


    void on_actionExport_Appointments_triggered();

    void on_actionImport_Appointments_triggered();

    void on_actionExport_Contacts_triggered();

    void on_actionImport_Contacts_triggered();


    void on_actionPreferences_triggered();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
