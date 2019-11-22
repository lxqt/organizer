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

#ifndef BIRTHDAYMODEL_H
#define BIRTHDAYMODEL_H

#include <QColor>
#include <QList>
#include <QDate>
#include <QDebug>
#include "birthday.h"
#include <QAbstractListModel>


class BirthdayModel: public QAbstractListModel
{
public:

    BirthdayModel(QObject* parent = nullptr);

    BirthdayModel(const QList<Birthday>& birthdayList,
                  QObject *parent = nullptr);

    void addBirthday(Birthday &birthday);
    Birthday getABirthday(int index);
    void clearAllBirthdays();
    void removeBirthday(int idx);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data (const QModelIndex & index,
                   int role = Qt::DisplayRole) const override;

private:
    QList<Birthday> modelBirthdayList;
};

#endif // BIRTHDAYMODEL_H
