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
#include <QTextCharFormat>
#include <QTimer>
#include <QList>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include<QLabel>

#include "dialogappointment.h"
#include "dialogcontact.h"
#include "dbmanager.h"
#include "appointment.h"
#include "contact.h"
#include "appointmentmodel.h"
#include "proxymodel.h"
#include "contactmodel.h"
#include "proxymodelcontacts.h"
#include "remindermodel.h"
#include "dialogabout.h"
#include "dbussessionmessage.h"
#include "birthday.h"
#include "birthdaymodel.h"
#include "reminder.h"

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

    QDate selectedDate; //from clicking on calendar

    //Appointments
    QString title="";
    QString location="";
    QString description="";
    QDate appointmentDate;
    QTime appointmentStartTime;
    QTime appointmentEndTime;
    QDate appointmentReminderDate;
    QTime appointmentReminderTime;
    int appointmentReminderRequested=0;

    int selectedAppointmentdDbId =0;

    Reminder reminderSingleShot;

    //Birthday (Keep Appointment format for display purposes)
    QString birthdayName="";
    QString birthdayLocation="";
    QString birthdayDescription="";
    QDate birthdayDate;
    int addToCalendar=1;


    int selectedBirthdayDbId=0;// may just toggle birthday display

    int selectedRowIdx=0;
    bool notificationsFlag;

    QTextCharFormat eventDayFormat;
    QTextCharFormat weekDayFormat;
    QTextCharFormat weekendFormat;

    QLabel *statusDateLabel;
    QLabel *statusTimeLabel;
    QTimer *timer;
    QTimer *timerSingleShot;

    //Contacts
    QString contactFirstName ="";
    QString contactLastName="";
    QString contactEmail="";    
    QString street="";
    QString city="";
    QString county="";
    QString postcode ="";
    QString country ="";
    QString phoneNumber="";
    QDate birthDate=QDate(); //(0,0,0)
    int birthDateId=0;


    int selectedContactDbId=0;
    Contact selectedContact;

    //Models

    AppointmentModel *dayModel;   
    ReminderModel *reminderModel;

    BirthdayModel *birthdayModel;
    BirthdayModel *birthdayReminderModel;
    ProxyModel *proxyModel;
    ContactModel *contactModel;
    ProxyModelContacts *proxyModelContacts;


    void NewAppointment();
    void UpdateAppointment(int dbID);
    void NewContact();
    void UpdateContact(int dbID);

    void LoadAppointmentsListFromDatabase(); //initialise AppointmentsList
    void LoadContactsListFromDatabase(); //initialise contactsList
    void LoadBirthdayListFromDatabase();
    void LoadReminderListFromDatabase();

    void DisplayContactsOnTableView();
    bool quickViewFlag=false;
    bool sortOrderFlag=false;

    void DisplayAppointmentsForDate(QDate theSelectedDate);
    void DisplayBirthdaysForDate(QDate theSelectedDate);

    //Calendar
    void setCalendarOptions();
    void showAllAppointmentsOnCalendar();
    void clearAllAppointmentsOnCalendar();
    void showAllBirthdaysOnCalendar();
    void clearAllBirthdaysOnCalendar();

    //Reminders
    void checkForReminders();    
    void checkForBirthdaysNextSevenDays();

    void exportContactsXML();
    void importContactsXML();
    void exportAppointmentsXML();
    void importAppointmentsXML();


private slots:

    void timerUpdateSlot();

    void sendReminderMessage();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_calendarWidget_clicked(const QDate &date);

    void on_actionNew_Appointment_triggered();

    void on_actionNew_Contact_triggered();

    void on_tableViewContacts_clicked(const QModelIndex &index);

    void on_tableViewContacts_doubleClicked(const QModelIndex &index);

    void on_pushButtonShowQuickDetails_clicked();   

    void on_pushButtonMailTo_clicked();

    void on_actionDelete_All_Contacts_triggered();

    void on_actionAbout_triggered();

    void on_actionClear_All_Reminder_Messages_triggered();

    void on_actionDelete_All_Appointments_2_triggered();

    void on_actionExit_triggered();

    void on_actionCalendar_Grid_triggered();   

    void on_actionCheckForReminders_triggered();

    void on_actionCheckForUpcomingBirthdays_triggered();

    void on_actionExport_Contacts_XML_triggered();

    void on_actionExport_Appointments_XML_triggered();

    void on_ActionImport_Contacts_XML_triggered();

    void on_actionImport_Appointments_XML_triggered();

    void on_actionCalendar_Weeks_triggered();

    void on_pushButtonSortByFirstLastname_clicked();

private:
    Ui::MainWindow *ui;

    QList<Appointment> appointmentsList;
    QList<Reminder> reminderList;    

    QList <Contact> contactsList;    
    QList<Birthday> birthdayList;

};

#endif // MAINWINDOW_H
