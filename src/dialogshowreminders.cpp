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

#include "dialogshowreminders.h"
#include "ui_dialogshowreminders.h"

DialogShowReminders::DialogShowReminders(QWidget *parent,
                                         QDate *theSelectedDate,
                                         DbManager *theDbm) :
    QDialog(parent),
    ui(new Ui::DialogShowReminders)
{
    ui->setupUi(this);
    setWindowTitle("Reminders");
    ui->labelSelectedDate->setText(theSelectedDate->toString());

    this->reminderList =theDbm->getAllReminders();

    QList<Reminder> dayReminderList =QList<Reminder>();

    foreach(Reminder r, reminderList)
    {
        QDate adate = QDate::fromString(r.m_reminderDate);

        if(adate==*theSelectedDate)
        {
            qDebug()<<"Reminder: "<<r.m_details;
            dayReminderList.append(r);

        }
    }
    reminderListModel = new remindersListModel(dayReminderList);


    ui->listView->setModel(reminderListModel);


}

DialogShowReminders::~DialogShowReminders()
{
    delete ui;
}
