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

#ifndef DIALOGREPEATAPPOINTMENT_H
#define DIALOGREPEATAPPOINTMENT_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "appointment.h"

namespace Ui {
class DialogRepeatAppointment;
}

class DialogRepeatAppointment : public QDialog
{
    Q_OBJECT

public:


    explicit DialogRepeatAppointment(QWidget *parent = nullptr, QDate *theAppointmentDate=nullptr);
    ~DialogRepeatAppointment();

    QDate appointmentDate;
    QTime startTime;
    QTime endTime;

    int isAllDay=0; //no
    int repeatDayInterval=0;
    int repeatNumber=0;

    int getRepeatDayInterval();
    int getRepeatNumber();

    QString getTitle();
    QString getLocation();
    QString getDescription();
    QDate getAppointmentDate();
    QTime getStartTime();
    QTime getEndTime();
    int getAllDay();

    //Translations
    QString t_dialog_title=QStringLiteral("Generate Repeat Appointments");
    QString t_start_date_display=QStringLiteral("Appointment Start Date");
    QString t_title=QStringLiteral("Title: ");
    QString t_location=QStringLiteral("Location: ");
    QString t_notes=QStringLiteral("Notes: ");
    QString t_all_day=QStringLiteral("All Day");
    QString t_start_time=QStringLiteral("Start Time: ");
    QString t_end_time=QStringLiteral("End Time: ");
    QString t_repeat_every=QStringLiteral("Repeat Every");
    QString t_days=QStringLiteral("Days");
    QString t_occurrences=QStringLiteral("Occurences");


    void setDialogTitleTranslation(QString translation);
    void setStartDateDisplayTranslation(QString translation);
    void setTitleTranslation(QString translation);
    void setLocationTranslation(QString translation);
    void setNotesTranslations(QString translation);
    void setAllDaytranslation(QString translation);
    void setStartTimeTranslation(QString translation);
    void setEndTimeTranslation(QString translation);
    void setRepeatEveryTranslation(QString translation);
    void setDaysTranslation(QString translation);
    void setOccurrencesTranslation(QString translation);

    void setLabelTranslations();

    QLocale locale=QLocale::English;
    void setLocaleDate(QLocale locale);


private slots:
    void accept();


    void on_timeEditStarts_userTimeChanged(const QTime &time);

    void on_timeEditEnds_userTimeChanged(const QTime &time);

    void on_checkBox_stateChanged(int arg1);

    void on_spinBoxDays_valueChanged(int arg1);

    void on_spinBoxRepeatNumber_valueChanged(int arg1);

private:
    Ui::DialogRepeatAppointment *ui;
};

#endif // DIALOGREPEATAPPOINTMENT_H
