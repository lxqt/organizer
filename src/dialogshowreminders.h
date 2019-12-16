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

#ifndef DIALOGSHOWREMINDERS_H
#define DIALOGSHOWREMINDERS_H

#include <QDialog>
#include <reminder.h>
#include <dbmanager.h>
#include <reminderslistmodel.h>

namespace Ui {
class DialogShowReminders;
}

class DialogShowReminders : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowReminders(QWidget *parent = nullptr,
                                 QDate *theSelectedDate=nullptr,
                                 DbManager *theDbm=nullptr);
    ~DialogShowReminders();
    QDate selectedDate;
    remindersListModel *reminderListModel;


private:
    Ui::DialogShowReminders *ui;
    QList<Reminder> reminderList;
    DbManager theDbm;
};

#endif // DIALOGSHOWREMINDERS_H
