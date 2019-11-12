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
#include "contactmodel.h"

ContactModel::ContactModel(QObject *parent):
    QAbstractTableModel(parent)
{
    Q_UNUSED(parent)
}

ContactModel::ContactModel(const QList<Contact> &contactList, QObject *parent)
{
    Q_UNUSED(parent)
    this->modelContactList=contactList;
}

void ContactModel::AddContact(Contact &contact)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    modelContactList.append(contact);
    endInsertRows();
}

Contact ContactModel::getContact(int index)
{
    Contact c = modelContactList.value(index);
    return c;
}

void ContactModel::clearAllContacts()
{
    beginResetModel();
    modelContactList.clear();
    endResetModel();
}

void ContactModel::removeContact(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelContactList.removeAt(idx);
    endRemoveRows();
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelContactList.size();
}

int ContactModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 11; //11 columns (see cases below)
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
           return QVariant();
       if (index.row() >= modelContactList.size() || index.row() < 0)
           return QVariant();


       if (role == Qt::DisplayRole) {
           const Contact& contact = modelContactList.at(index.row());

           QString birthdayTimestamp =contact.m_birthdayTimestamp;
           QDateTime birthdaydatetime =QDateTime::fromString(birthdayTimestamp);
           QDate birthdayDate =birthdaydatetime.date();

           QString birthdayDateStr="("+QString::number(birthdayDate.day())+"/"
                   +QString::number(birthdayDate.month())+"/"
                   +QString::number(birthdayDate.year())+")";


           switch (index.column()) {
           case 0:
               return contact.m_id;
           case 1:
               return contact.m_firstname;
           case 2:
               return contact.m_lastname;
           case 3:
               return contact.m_email; //format to 2 decimal place
           case 4:
               return contact.m_street;
           case 5:
               return contact.m_city;
           case 6:
               return contact.m_county;
           case 7:
               return contact.m_postcode;
           case 8:
               return contact.m_country;
           case 9:
               return contact.m_telephone;
           case 10:
               return birthdayDateStr;

           default:
               return QVariant();
           }
       }
       else if (role == Qt::TextAlignmentRole){
           if (index.column() == 3 || index.column()==4) //start and end times
               return Qt::AlignCenter;
           else
               return Qt::AlignLeft;
       }
       return QVariant();
}



QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "ContactId";
        case 1:
            return "First Name";
        case 2:
            return "Last Name";
        case 3:
            return "Email";
        case 4:
            return "Street";
        case 5:
            return "City";
        case 6:
            return "County";
        case 7:
            return "Postcode";
        case 8:
            return "Country";
        case 9:
            return "Telephone";
        case 10:
            return "Birthday";
        default:
            return QVariant();
        }
    }
    return section + 1;
}

