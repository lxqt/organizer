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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QTextCharFormat>
#include "dialogabout.h"
#include "event.h"
#include "eventmodel.h"
#include "dbmanager.h"
#include "dbussessionmessage.h"
#include "dialogaddevent.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    DbManager dbm;
    //QDate appointmentDate;
    EventModel eventModel;
    QStringListModel *model; //use model-view architecture
    int day=0;
    int month=0;
    int year=0;
    int startTime=0;
    int endTime=0;
    //int id=0;
    QDate selectedDate;
    int selectedDbId =0;
    int selectedRowIdx=0;
    QDate reminderDate;
    int remDay=0;
    int remMonth=0;
    int remYear=0;
    int remTime=0;
    int reminderDays=0;
    QString title="";
    QString location="";
    void initialiseDates();
    void showDbEventsAll();
    void ShowDbEventsForDate(QDate &dairyDate);
    void addEvent();
    QList<Event> getAllAppointments();
    bool notificationsFlag;
    void setDefaultOptions();
    void setCalendarOptions();
    QTextCharFormat eventDayFormat;
    QTextCharFormat weekDayFormat;
    QTextCharFormat weekendFormat;
    void resetCalendarColours();




private slots:

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_calendarWidget_clicked(const QDate &date);

    void on_actionDelete_All_triggered();   

    void on_listView_clicked(const QModelIndex &index);

    void on_actionNotifications_triggered();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_actionClear_List_triggered();

    void on_actionCalendar_Grid_triggered();

    void on_actionCalendar_Weeks_triggered();

    void on_actionAdd_triggered();

    void on_actionShow_All_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
