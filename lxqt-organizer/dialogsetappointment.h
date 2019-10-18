#ifndef DIALOGSETAPPOINTMENT_H
#define DIALOGSETAPPOINTMENT_H

#include <QDialog>
#include <QDate>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class DialogSetAppointment;
}

class DialogSetAppointment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetAppointment(QWidget *parent = nullptr, QDate *eventDate=nullptr);
    ~DialogSetAppointment();
    QString getTitle();
    QString getLocation();
    int getStartTime();
    int getEndTime();
    QDate getReminderDate();
    int getReminderTime();
    //QDate startDate;
    //QDate endDate;
    int startTime=0;
    int endTime=0;
    int reminderDays=0;
    int reminderTime=0;
    QDate eventDate;
    QDate reminderDate;
    void setupComboBoxes();

private slots:
    void on_comboBoxStartTime_activated(int index);

    void on_comboBoxEndTime_activated(int index);

    void on_comboBoxReminder_activated(int index);

    void on_comboBoxReminderTime_activated(int index);

private:
    Ui::DialogSetAppointment *ui;
};

#endif // DIALOGSETAPPOINTMENT_H
