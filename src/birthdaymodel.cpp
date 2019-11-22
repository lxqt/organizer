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



#include "birthdaymodel.h"


BirthdayModel::BirthdayModel(QObject *parent)
{
    Q_UNUSED(parent)
    this->modelBirthdayList= QList<Birthday>();

}

BirthdayModel::BirthdayModel(const QList<Birthday> &birthdayList, QObject *parent)
{
    Q_UNUSED(parent)
    this->modelBirthdayList=birthdayList;
}

void BirthdayModel::addBirthday(Birthday &birthday)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->modelBirthdayList.append(birthday);
    endInsertRows();
}

Birthday BirthdayModel::getABirthday(int index)
{
    Birthday b = modelBirthdayList.value(index);
    return b;
}

void BirthdayModel::clearAllBirthdays()
{
    beginResetModel();
    modelBirthdayList.clear();
    endResetModel();

}

void BirthdayModel::removeBirthday(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelBirthdayList.removeAt(idx);
    endRemoveRows();
}

int BirthdayModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelBirthdayList.size();
}

QVariant BirthdayModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelBirthdayList.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {

        const Birthday& birthday = modelBirthdayList.at(index.row());

        QDate currentDate=QDate::currentDate();
        QDate bornDate =QDate::fromString(birthday.m_birthDate);
        QDate birthdayDate=QDate(currentDate.year(),bornDate.month(),bornDate.day());

        int birthdayDay =birthdayDate.day();
        int birthdayMonth =birthdayDate.month();
        int birthdayYear =birthdayDate.year();

        QString dateStr="("+QString::number(birthdayDay)+"/"
                +QString::number(birthdayMonth)+"/"
                +QString::number(birthdayYear)+")";

        QString details="Birthday: "+birthday.m_name
                +" ("+birthday.m_location
                + ") "+dateStr;


        return details;
    }
      return QVariant();
}


