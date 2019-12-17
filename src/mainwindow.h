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
#include "holiday.h"
#include "dbmanager.h"

#include "dialogappointment.h"
#include "dialogcontact.h"
#include "dialogshowreminders.h"
#include "daylistmodel.h"
#include "contactmodel.h"
#include "proxymodelcontacts.h"
#include "dbussessionmessage.h"
#include "dialogabout.h"



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
    int birthDateAppointmentId=0;
    int addBirthdayToCalendar =0;

    int selectedContactDbId=0;
    Contact selectedContact;

    //Models

    ContactModel *contactModel;
    ProxyModelContacts *proxyModelContacts;

    //Appointments    
    void newAppointment();
    void updateAppointment(int dbID);
    void LoadAppointmentsListFromDatabase();
    void UpdateAppointment(int dbID);
    static bool compare(const Appointment& first, const Appointment& second);
    void ShowDayAppointments();

    void RemoveAllAppointments();
    void RemoveAllBirthdayAppointmentsFromDatebase();

    //Contacts
    void LoadContactsListFromDatabase();
    void NewContact();
    void UpdateContact(int dbID);
    void DisplayContactsOnTableView();
    bool quickViewFlag=false;
    bool sortOrderFlag=false;

     void AddBirthdayAppointmentsToDatabase(int year); //uses contact birth


    //Reminders
    void LoadReminderListFromDatabase();
    void checkForRemindersMidday();
    void checkForReminders();
    void DisplayRemindersForDate(QDate date);
    void checkForBirthdaysNextSevenDays();

    bool notificationsFlag=false;

    //Holidays

    void RemoveAllHolidayAppointmentsFromDatabase();
    void AddHolidayAppointmentsToDatabase(int year);

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

    void on_listViewDay_doubleClicked(const QModelIndex &index);

    void on_actionNew_Contact_triggered();

    void on_tableViewContacts_doubleClicked(const QModelIndex &index);

    void on_actionClear_All_Appointments_triggered();

    void on_action_Add_Current_Birthdays_triggered();

    void on_actionRemove_All_Birthday_triggered();

    void on_pushButtonShowQuickFullDetails_clicked();

    void on_pushButtonSort_clicked();

    void on_pushButtonMailTo_clicked();

    void on_tableViewContacts_clicked(const QModelIndex &index);

    void on_actionCheck_For_Upcoming_Birthdays_triggered();

    void on_actionAbout_triggered();

    void on_actionDelete_All_Contacts_triggered();

    void on_actionExport_Contacts_XML_triggered();

    void on_actionExport_Appointments_XML_triggered();

    void on_actionImport_Contacts_XML_triggered();

    void on_actionImport_Appointments_XML_triggered();

    void on_actionCheck_For_Reminders_Today_triggered();

    void on_actionSystem_Notifications_triggered();



private:
    Ui::MainWindow *ui;
    void gotoNextMonth();
    void gotoPreviousMonth();
    void updateCalendar();

    const char *monthNames[12]= {"January", "February", "March", "April",
                                 "May", "June", "July", "August", "September",
                                 "October", "November", "December"};
    QTableWidgetItem* dayItem;
    QTableWidgetItem* appointmentItem;

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
    QList <Contact> contactsList;
    QList<Holiday> holidayList;
    //Models
    DayListModel *dayListModel;

};


#endif // MAINWINDOW_H
