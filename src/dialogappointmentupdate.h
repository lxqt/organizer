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


    //Appointments
    QString title="";
    QString location="";
    QString description="";
    QDate appointmentDate;
    QTime appointmentStartTime;
    QTime appointmentEndTime;
    QString category="";
    int categoryIndex=0;
    int isAllDay=0;

    QTime startTime;
    QTime endTime;

    bool deleteRequested=false;
    bool valid =false;

    QString currentTitle="";
    QString getTitle();
    QString getLocation();
    QString getDescription();
    QDate getAppointmentDate();
    QTime getStartTime();
    QTime getEndTime();
    int getAllDay();
    QString getCategory();

    bool getDeleteRequested();
    void setupComboBoxes();

    //reminders
    int reminderMins=0;
    int getReminderMins();
    int hasReminder=0;
    int getHasReminder();

private slots:

    void accept();
    void reject();



    void on_comboBoxCategory_activated(int index);

    void on_lineEditTitle_textChanged(const QString &arg1);

    void on_lineEditLocation_textChanged(const QString &arg1);

    void on_textEditDescription_textChanged();

    void on_checkBoxAllDay_stateChanged(int arg1);

    void on_timeEditStartTime_userTimeChanged(const QTime &time);

    void on_timeEditEndTime_userTimeChanged(const QTime &time);

    void on_checkBoxDelete_stateChanged(int arg1);

    void on_comboBoxCategory_currentIndexChanged(const QString &arg1);

    void on_comboBoxReminder_currentIndexChanged(int index);

    void on_checkBoxReminder_stateChanged(int arg1);

private:
    Ui::DialogAppointmentUpdate *ui;
};

#endif // DIALOGAPPOINTMENTUPDATE_H
