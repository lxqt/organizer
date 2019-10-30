#include "remindermodel.h"


ReminderModel::ReminderModel(QObject *parent)
{
    Q_UNUSED(parent)
    this->modelReminderList= QList<Appointment>();

}

ReminderModel::ReminderModel(const QList<Appointment> &appointmentList, QObject *parent)
{
    Q_UNUSED(parent)
   this->modelReminderList=appointmentList;
}

void ReminderModel::addReminderAppointment(Appointment &appointment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->modelReminderList.append(appointment);
    endInsertRows();
}

Appointment ReminderModel::getAReminderAppointment(int index)
{
    Appointment a = modelReminderList.value(index);
    return a;
}

void ReminderModel::clearAllReminderAppointments()
{
    beginResetModel();
    modelReminderList.clear();
    endResetModel();
}

void ReminderModel::removeReminderAppointment(int idx)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    modelReminderList.removeAt(idx);
    endRemoveRows();
}

int ReminderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelReminderList.size();
}

QVariant ReminderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelReminderList.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {

        const Appointment& appointment = modelReminderList.at(index.row());

        QString dateStr="("+QString::number(appointment.m_day)+"/"
                +QString::number(appointment.m_month)+"/"
                +QString::number(appointment.m_year)+")";

        QString reminderDetails=appointment.m_title
                +" ("+appointment.m_location
                        + ") "+dateStr+" "
                 +QString::number(appointment.m_startTime)+":00";

        return reminderDetails;
    }

        return QVariant();
}
