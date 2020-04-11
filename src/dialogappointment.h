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


#ifndef DIALOGAPPOINTMENT_H
#define DIALOGAPPOINTMENT_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "appointment.h"

namespace Ui {
class DialogAppointment;
}

class DialogAppointment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAppointment(QWidget *parent = nullptr, QDate *theAppointmentDate=nullptr);

    ~DialogAppointment();

    //Appointment appointment;

    QDate appointmentDate;
    QTime startTime;
    QTime endTime;
    QString category;    

    int isAllDay=0; //no
    bool deleteRequested=false;
    QString currentTitle=QStringLiteral("");

    QString getTitle();
    QString getLocation();
    QString getDescription();
    QDate getAppointmentDate();
    QTime getStartTime();
    QTime getEndTime(); 
    int getAllDay();


    bool getDeleteRequested();
    void setupComboBoxes();

    //Reminders
    int hasReminder=0; //no
    int getHasReminder();
    int reminderMins=0; //none
    int getReminderMins();

    //Set externally with setLabelTranslations();
    QString t_dialog_title;
    QString t_date_display;
    QString t_title;
    QString t_location;
    QString t_notes;
    QString t_all_day;
    QString t_start_time;
    QString t_end_time;

    QString t_reminder;

    QString t_reminder_5min;
    QString t_reminder_10min;
    QString t_reminder_30min;
    QString t_reminder_1hour;
    QString t_reminder_1day;

    void setDialogTitleTranslation(QString translation);
    void setDateDisplayTranslation(QString translation);
    void setTitleTranslation(QString translation);
    void setLocationTranslation(QString translation);
    void setNotesTranslations(QString translation);
    void setAllDaytranslation(QString translation);
    void setStartTimeTranslation(QString translation);
    void setEndTimeTranslation(QString translation);   
    void setReminderTranslation(QString translation);

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

    void on_timeEditStartTime_userTimeChanged(const QTime &time);

    void on_timeEditEndTime_userTimeChanged(const QTime &time);

    void on_checkBoxAllDay_stateChanged(int arg1);

    void on_comboBoxReminder_currentIndexChanged(int index);

    void on_checkBoxReminder_stateChanged(int arg1);

private:
    Ui::DialogAppointment *ui;
};

#endif // DIALOGAPPOINTMENT_H
