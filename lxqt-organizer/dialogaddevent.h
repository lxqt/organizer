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


#ifndef DIALOGADDEVENT_H
#define DIALOGADDEVENT_H


#include <QDialog>
#include <QDate>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class DialogAddEvent;
}

class DialogAddEvent : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddEvent(QWidget *parent = nullptr,  QDate *eventDate=nullptr);
    ~DialogAddEvent();
    QString getTitle();
    QString getLocation();
    QDate getEventDate();
    int getStartTime();
    int getEndTime();
    QDate getReminderDate();
    int getReminderTime();

    QDate eventDate;
    int startTime=0;
    int endTime=0;

    QDate reminderDate;
    int reminderTime=0;
    int reminderDays=0;

    void setupComboBoxes();

private slots:


    void on_dateEdit_userDateChanged(const QDate &date);

    void on_comboBoxStartTime_activated(int index);

    void on_comboBoxEndTime_activated(int index);

    void on_comboBoxReminderDays_activated(int index);

    void on_comboBoxReminderTime_activated(int index);

private:
    Ui::DialogAddEvent *ui;
};

#endif // DIALOGADDEVENT_H
