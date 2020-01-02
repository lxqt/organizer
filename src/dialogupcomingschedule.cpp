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

#include "dialogupcomingschedule.h"
#include "ui_dialogupcomingschedule.h"


DialogUpcomingSchedule::DialogUpcomingSchedule(QWidget *parent,
                                               DbManager *theDbm) :
    QDialog(parent),
    ui(new Ui::DialogUpcomingSchedule)
{
     ui->setupUi(this);

     setWindowTitle("Upcoming Schedule (Next Seven Days)");

     this->appointmentList= theDbm->getAllAppointments();
     this->theDbm=*theDbm;

     QList<Appointment> schedule =QList<Appointment>();

     QDate currentDate =QDate::currentDate();
     ui->labelDate->setText(currentDate.toString());

     for(int i=1; i<8; i++)
     {
         foreach(Appointment a, appointmentList)  //use contacts!!
         {
             QDate appdate = QDate::fromString(a.m_date);
             if (appdate.addDays(-i)==currentDate) {                
                 schedule.append(a);
             }
         }
     }
     scheduleListModel = new ScheduleListModel(schedule);
     ui->listViewSchedule->setModel(scheduleListModel);
}

DialogUpcomingSchedule::~DialogUpcomingSchedule()
{
    delete ui;
}
