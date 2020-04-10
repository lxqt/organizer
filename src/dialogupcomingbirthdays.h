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

#ifndef DIALOGUPCOMINGBIRTHDAYS_H
#define DIALOGUPCOMINGBIRTHDAYS_H

#include <QDialog>
#include <QList>
#include <QDate>
#include <QDebug>
#include "contact.h"
#include "birthdaylistmodel.h"

namespace Ui {
class DialogUpcomingBirthdays;
}

class DialogUpcomingBirthdays : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUpcomingBirthdays(QWidget *parent = nullptr,
                                     QList<Contact> *contactList=nullptr);
    ~DialogUpcomingBirthdays();
    QString t_title=QStringLiteral("Upcoming Birthdays (Next Seven Days)");
    void setTitleTranslation(QString translation);

private:
    Ui::DialogUpcomingBirthdays *ui;
    QList<Contact> dialogContactList;
    QList<QString> birthdayList;
    BirthdayListModel *birthdayListModel;
};

#endif // DIALOGUPCOMINGBIRTHDAYS_H
