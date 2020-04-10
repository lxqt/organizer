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

#ifndef DIALOGAPPOINTMENTUPDATE_H
#define DIALOGAPPOINTMENTUPDATE_H

#include <QDialog>
#include <QDialog>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "appointment.h"
namespace Ui {
class DialogAppointmentUpdate;
}

class DialogAppointmentUpdate : public QDialog
{
    Q_OBJECT

public:

    explicit DialogAppointmentUpdate(QWidget *parent = nullptr, Appointment *theAppointment=nullptr);
    ~DialogAppointmentUpdate();



    //Appointment values (default)
    QString title=QStringLiteral("");
    QString location=QStringLiteral("");
    QString description=QStringLiteral("");
    QDate appointmentDate;
    QTime appointmentStartTime;
    QTime appointmentEndTime;

    int isAllDay=0;

    QTime startTime;
    QTime endTime;

    bool deleteRequested=false;
    bool valid =false;

   //QString currentTitle=QStringLiteral("");

    QString getTitle();
    QString getLocation();
    QString getDescription();
    QDate getAppointmentDate();
    QTime getStartTime();
    QTime getEndTime();
    int getAllDay();

    bool getDeleteRequested();
    void setupComboBoxes();

    //reminders
    int reminderMins=0;
    int getReminderMins();
    int hasReminder=0;
    int getHasReminder();

    //Translations
    QString t_dialog_update=QStringLiteral("New Appointment");
    QString t_date_display=QStringLiteral("Appointment Date");
    QString t_title=QStringLiteral("Title: ");
    QString t_location=QStringLiteral("Location: ");
    QString t_notes=QStringLiteral("Notes: ");
    QString t_all_day=QStringLiteral("All Day");
    QString t_start_time=QStringLiteral("Start Time: ");
    QString t_end_time=QStringLiteral("End Time: ");
    QString t_category=QStringLiteral("Category");
    QString t_reminder=QStringLiteral("Reminder");
    QString t_delete=QStringLiteral("Delete Appointment");


    QString t_reminder_5min=QStringLiteral("5 minutes before start");
    QString t_reminder_10min=QStringLiteral("10 minutes before start");
    QString t_reminder_30min=QStringLiteral("30 minutes before start");
    QString t_reminder_1hour=QStringLiteral("1 hour before start");
    QString t_reminder_1day=QStringLiteral("1 day before start");

    void setDialogUpdateTranslation(QString translation);
    void setDateDisplayTranslation(QString translation);
    void setTitleTranslation(QString translation);
    void setLocationTranslation(QString translation);
    void setNotesTranslations(QString translation);
    void setAllDaytranslation(QString translation);
    void setStartTimeTranslation(QString translation);
    void setEndTimeTranslation(QString translation);
    void setCategoryTranslation(QString translation);
    void setReminderTranslation(QString translation);
    void setDeleteTranslation(QString translation);
    void setLabelTranslations();

    void setReminder5MinTranslation(QString translation);
    void setReminder10MinTranslation(QString translation);
    void setReminder30MinTranslation(QString translation);
    void setReminder1HourTranslation(QString translation);
    void setReminder1DayTranslation(QString translation);

    void setComboBoxReminderTranslations();

    QLocale locale=QLocale::English;
    void setLocaleDate(QLocale locale);

private slots:

    void accept();
    void reject();

    void on_lineEditTitle_textChanged(const QString &arg1);

    void on_lineEditLocation_textChanged(const QString &arg1);

    void on_textEditDescription_textChanged();

    void on_checkBoxAllDay_stateChanged(int arg1);

    void on_timeEditStartTime_userTimeChanged(const QTime &time);

    void on_timeEditEndTime_userTimeChanged(const QTime &time);

    void on_checkBoxDelete_stateChanged(int arg1);  

    void on_comboBoxReminder_currentIndexChanged(int index);

    void on_checkBoxReminder_stateChanged(int arg1);

private:
    Ui::DialogAppointmentUpdate *ui;
};

#endif // DIALOGAPPOINTMENTUPDATE_H
