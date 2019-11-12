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
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QTextCharFormat>
#include <QTimer>
#include <QList>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamWriter>

#include "dialogaddappointment.h"
#include "dialogaddcontact.h"
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

    QDate selectedDate;

    QString title="";
    QString location="";
    QString description="";

    QTime appointmentStartTime;
    QTime appointmentEndTime;
    QDateTime appointmentStartDateTime;
    QDateTime appointmentEndDateTime;
    QString appointmentStartTimestamp;
    QString appointmentEndTimestamp;

    Appointment reminderAppointment;

    QDate reminderDate;
    QTime reminderTime;
    QDateTime reminderDateTime;
    QString reminderTimestamp;
    int reminderRequested=0;

    int selectedAppointmentdDbId =0;
    int selectedRowIdx=0;
    bool notificationsFlag;
    QTextCharFormat eventDayFormat;
    QTextCharFormat weekDayFormat;
    QTextCharFormat weekendFormat;
    QLabel *statusDateLabel;
    QLabel *statusTimeLabel;
    QTimer *timer;
    QTimer *timerSingleShot;



    QString contactFirstName ="";
    QString contactLastName="";
    QString contactEmail="";
    QDate birthdayDate=QDate();
    QString street="";
    QString city="";
    QString county="";
    QString postcode ="";
    QString country ="";
    QString phoneNumber="";
    QDateTime birthdayDateTime;
    QString birthdayTimestamp;
    QDate birthdayReminderDate;
    QTime birthdayReminderTime;
    QDateTime birthdayReminderDateTime;
    int birthdayReminderRequest=0;
    QString birthdayReminderTimestamp;
    int selectedContactDbId=0;
    Contact selectedContact;

    //Models
    AppointmentModel *appointmentModel;
    AppointmentModel *dayModel;
    AppointmentModel *birthdayModel;
    ReminderModel *reminderModel;
    ProxyModel *proxyModel;
    ContactModel *contactModel;
    ProxyModelContacts *proxyModelContacts;


    void AddAppointment();
    void AddContact();

    void LoadAppointmentsListFromDatabase(); //initialise AppointmentsList
    void LoadContactsListFromDatabase(); //initialise contactsList

    void DisplayContactsOnTableView();
    bool quickViewFlag=false;


    void UpdateDisplayList();
    void DisplayEventsForDate(QDate theSelectedDate);

    //Calendar
    void setCalendarOptions();
    void showAllEventsOnCalendar();
    void clearAllEventsOnCalendar();

    //Reminders
    void checkForReminders();




private slots:
      void timerUpdateSlot();

      void sendReminderMessage();

      void on_actionNew_Appointment_triggered();

      void on_actionNew_Contact_triggered();

      void on_actionDelete_All_Contacts_triggered();

      void on_actionDelete_All_Appointments_2_triggered();

      void on_actionExit_triggered();

      void on_tableView_doubleClicked(const QModelIndex &index);

      void on_tableViewContacts_doubleClicked(const QModelIndex &index);

      void on_tableViewContacts_clicked(const QModelIndex &index);

      void on_pushButtonShowQuickDetails_clicked();



      void on_pushButtonMailTo_clicked();

      void on_actionExport_Contacts_XML_triggered();

      void on_calendarWidget_clicked(const QDate &date);

      void on_pushButtonSearchByLastname_clicked();

      void on_actionExport_Appointments_XML_triggered();

      void on_listViewReminders_clicked(const QModelIndex &index);

      void on_actionClear_All_Reminder_Messages_triggered();

      void on_actionAbout_triggered();

      void on_actionSystem_Notifications_triggered();

      void on_actionCalendar_Grid_triggered();

      void on_actionCalendar_Weeks_triggered();

private:
    Ui::MainWindow *ui;
    QList<Appointment> appointmentsList;
    QList <Contact> contactsList;
    QList<Appointment> displayList;
    QXmlStreamWriter xml;
};

#endif // MAINWINDOW_H
