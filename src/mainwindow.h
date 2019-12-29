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
#include <QMainWindow>
#include <QMessageBox>
#include <QTextEdit>
#include <QDebug>
#include <QDate>
#include <QTimer>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QDomDocument>


#include "appointment.h"
#include "reminder.h"
#include "contact.h"

#include "dbmanager.h"

#include "dialogappointment.h"
#include "dialogcontact.h"
#include "dialogshowreminders.h"
#include "contactmodel.h"
#include "proxymodelcontacts.h"
#include "dbussessionmessage.h"
#include "dialogabout.h"

#include "daymodel.h"
#include "proxymodelappointments.h"



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
    QString title=QLatin1String("");
    QString location=QLatin1String("");
    QString description=QLatin1String("");
    QDate appointmentDate;
    QTime appointmentStartTime;
    QTime appointmentEndTime;
    QString category=QLatin1String("");
    int reminderDays=0;
    int reminderRequested=0;
    int isAllDay=0;
    int selectedAppointmentdDbId =0;

    Reminder reminderSingleShot;


    //Timers
    QLabel *statusDateLabel;
    QLabel *statusTimeLabel;
    QTimer *timer;
    QTimer *timerSingleShot;

    //Contacts
    QString contactFirstName =QLatin1String("");
    QString contactMiddleNames=QLatin1String("");
    QString contactLastName=QLatin1String("");
    QString contactEmail=QLatin1String("");
    QString street=QLatin1String("");
    QString district=QLatin1String("");
    QString city=QLatin1String("");
    QString county=QLatin1String("");
    QString postcode =QLatin1String("");
    QString country =QLatin1String("");
    QString phoneNumber=QLatin1String("");
    QDate birthDate=QDate(); //(0,0,0)
    int birthDateAppointmentId=0;
    int addBirthdayToCalendar =0;

    int selectedContactDbId=0;
    Contact selectedContact;

    //Models

    ContactModel *contactModel;
    ProxyModelContacts *proxyModelContacts;

    //Appointments    
    void NewAppointment();
    void UpdateAppointment(int dbID);
    void ShowDayAppointmentsTableView();

    //AppointmentList methods
    void LoadDatabaseAppointmentsToAppointmentList();
    void UpdateAppointmentInAppointmentList(const Appointment &app, int appointmentId);
    void RemoveAppointmentFromAppointmentList(int appointmentId);

    //Update Lists
    void UpdateAppointmentList();
    void UpdateReminderList();

    //ReminderList methods
    void LoadDatabaseRemindersToRemindersList();
    void UpdateReminderInReminderList(const Reminder &rem, int appointmentId);
    void RemoveReminderFromReminderList(int appointmentId);


    void RemoveAllAppointments();

    void CheckForRemindersOnHour();
    void checkForReminders();
    void DisplayRemindersForDate(QDate date);
    void checkForBirthdaysNextSevenDays();

    //Flags
    bool flagNotifications=false;
    bool flagQuickView=false;
    bool flagRemindersHourly =true;



    //Contacts
    void LoadDatebaseContactsToContactList();
    void NewContact();
    void UpdateContact(int dbID);
    void DisplayContactsOnTableView();

    //ContactList methods
    void RemoveContactFromContactList(int contactId);

    //Birthdays
    void AddBirthdaysToAppointmentList(int year);
    void UpdateBirthdaysInAppointmentList(int year);

    void RemoveBirthdayFromAppointmentList(int contactId);
    void UpdateBirthdayInAppointmentsList(const Appointment &b, int contactId);




    //Holidays
    void AddHolidaysToAppointmentsList(int year);
    void UpdateHolidaysInAppointmentList(int year);
    void RemoveHolidaysFromAppointmentsList();

    QDate CalculateEaster(int year);


    //exports and imports

    void exportContactsXML();
    void importContactsXML();
    void exportAppointmentsXML();
    void importAppointmentsXML();

private slots:

    void timerUpdateSlot();

    void sendReminderMessage();

    void on_actionExit_triggered();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButtonNextMonth_clicked();

    void on_pushButtonPreviousMonth_clicked();

    void on_actionNew_Appointment_triggered();    

    void on_actionCheck_For_Reminders_triggered();    

    void on_actionNew_Contact_triggered();

    void on_tableViewContacts_doubleClicked(const QModelIndex &index);    

    void on_pushButtonShowQuickFullDetails_clicked();

   // void on_pushButtonSort_clicked();

    void on_pushButtonMailTo_clicked();

    void on_actionCheck_For_Upcoming_Birthdays_triggered();

    void on_actionAbout_triggered();

    void on_actionDelete_All_Contacts_triggered();

    void on_actionExport_Contacts_XML_triggered();

    void on_actionExport_Appointments_XML_triggered();

    void on_actionImport_Contacts_XML_triggered();

    void on_actionImport_Appointments_XML_triggered();

    void on_actionCheck_For_Reminders_Today_triggered();

    void on_actionSystem_Notifications_triggered();

    void on_tableViewDays_doubleClicked(const QModelIndex &index);

    void on_actionDelete_All_Appointments_triggered();

    void on_tableViewContacts_clicked(const QModelIndex &index);

   // void on_pushButtonSortAscending_clicked();

    void on_actionCheck_Reminders_Hourly_triggered();

private:
    Ui::MainWindow *ui;
    void gotoNextMonth();
    void gotoPreviousMonth();
    void UpdateCalendar();

    const char *monthNames[12]= {"January", "February", "March", "April",
                                 "May", "June", "July", "August", "September",
                                 "October", "November", "December"};
    QTableWidgetItem* dayItem;
    QTableWidgetItem* appointmentItem;
    QTableWidgetItem* birthdayItem;

    int columnCount=7;
    int rowCount=6;
    //int dayArray[6][7];
    int dayArray[6*7];
    int selectedYear=0;
    int selectedMonth=0;
    int selectedDay=0;
    int firstDay=1;
    int fontSize=11;
    QDate selectedDate;
    QList<Appointment> appointmentList;
    QList<Reminder> reminderList;
    QList <Contact> contactList;
    //Models

    DayModel *dayModel;
    ProxyModelAppointments *proxyModelAppointments;

};


#endif // MAINWINDOW_H
