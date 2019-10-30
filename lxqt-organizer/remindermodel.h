#ifndef REMINDERMODEL_H
#define REMINDERMODEL_H

#include <QColor>
#include <QList>
#include <QDate>
#include <QDebug>
#include "appointment.h"
#include <QAbstractListModel>

class ReminderModel: public QAbstractListModel
{
public:
    ReminderModel(QObject* parent = nullptr);

    ReminderModel(const QList<Appointment>& appointmentList,
                                  QObject *parent = nullptr);

    void addReminderAppointment(Appointment &appointment);
    Appointment getAReminderAppointment(int index);
    void clearAllReminderAppointments();
    void removeReminderAppointment(int idx);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data (const QModelIndex & index,
                           int role = Qt::DisplayRole) const override;


private:
    QList<Appointment> modelReminderList;



};

#endif // REMINDERMODEL_H
