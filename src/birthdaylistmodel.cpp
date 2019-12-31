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

#include "birthdaylistmodel.h"

BirthdayListModel::BirthdayListModel(QObject *parent)
{
    Q_UNUSED(parent)
    this->modelBirthdayList= QList<QString>();
}

BirthdayListModel::BirthdayListModel(const QList<QString> &birthdayList, QObject *parent)
{
    Q_UNUSED(parent)
    this->modelBirthdayList=birthdayList;

}

void BirthdayListModel::addBirthday(QString &birthday)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->modelBirthdayList.append(birthday);
    endInsertRows();
}

int BirthdayListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelBirthdayList.size();

}

QVariant BirthdayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelBirthdayList.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {

        QString details="";
        details = modelBirthdayList.at(index.row());
        return details;
    }
    return QVariant();
}
