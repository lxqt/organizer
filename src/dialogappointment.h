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

#ifndef DIALOGAPPOINTMENT_H
#define DIALOGAPPOINTMENT_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "appointment.h"
#include "reminder.h"


namespace Ui {
class DialogAppointment;
}

class DialogAppointment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAppointment(QWidget *parent = nullptr, QDate *theAppointmentDate=nullptr);
    explicit DialogAppointment(QWidget *parent = nullptr, Appointment *theAppointment=nullptr);
    ~DialogAppointment();
    QDate appointmentDate;
    QTime startTime;
    QTime endTime;
    QString category;

    QDate reminderDate;
    QTime reminderTime;
    int reminderDays=1;
    int reminderRequested=0;

    int isAllDay=0; //no
    bool deleteRequested=false;

    QString getTitle();
    QString getLocation();
    QString getDescription();
    QDate getAppointmentDate();
    QTime getStartTime();
    QTime getEndTime();
    int getReminderDays();
    int getAllDay();
    QString getCategory();
    int getReminderRequested();

    bool getDeleteRequested();
    void setupComboBoxes();

private slots:
    void accept();

    void on_dateEditAppointmentDate_userDateChanged(const QDate &date);

    void on_timeEditStartTime_userTimeChanged(const QTime &time);

    void on_timeEditEndTime_userTimeChanged(const QTime &time);

    void on_checkBoxDelete_stateChanged(int arg1);   

    void on_comboBoxCategory_activated(const QString &arg1);

    void on_checkBoxAllDay_stateChanged(int arg1);

    void on_comboBoxReminder_activated(int index);

    void on_checkBoxReminder_stateChanged(int arg1);

private:
    Ui::DialogAppointment *ui;
};

#endif // DIALOGAPPOINTMENT_H
