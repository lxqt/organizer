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

     setWindowTitle(tr("Upcoming Schedule (Next Seven Days)"));

     this->appointmentList= theDbm->getAllAppointments();
     this->theDbm=*theDbm;

     QList<Appointment> schedule =QList<Appointment>();
     //QList<Appointment> dayList =QList<Appointment>();

     QDate currentDate =QDate::currentDate();
     ui->labelDate->setText(currentDate.toString());

     QDate currentDatePlusOne=currentDate.addDays(1);
     QDate currentDatePlusTwo=currentDate.addDays(2);
     QDate currentDatePlusThree=currentDate.addDays(3);
     QDate currentDatePlusFour=currentDate.addDays(4);
     QDate currentDatePlusFive=currentDate.addDays(5);
     QDate currentDatePlusSix=currentDate.addDays(6);
     QDate currentDatePlusSeven=currentDate.addDays(7);

     //Get the dayslist for currentDatePlusOne
     QList<Appointment> dayList1 =QList<Appointment>();
     foreach(Appointment a, appointmentList)
     {
         QDate adate = QDate::fromString(a.m_date);

         if(adate==currentDatePlusOne)
         {
             dayList1.append(a);
         }
     }
     std::sort(dayList1.begin(), dayList1.end(), compare);
     schedule.append(dayList1);

     QList<Appointment> dayList2 =QList<Appointment>();
     foreach(Appointment a, appointmentList)
     {
         QDate adate = QDate::fromString(a.m_date);

         if(adate==currentDatePlusTwo)
         {
             dayList2.append(a);
         }
     }
     std::sort(dayList2.begin(), dayList2.end(), compare);
     schedule.append(dayList2);

     QList<Appointment> dayList3 =QList<Appointment>();
     foreach(Appointment a, appointmentList)
     {
         QDate adate = QDate::fromString(a.m_date);

         if(adate==currentDatePlusThree)
         {
             dayList3.append(a);
         }
     }
     std::sort(dayList3.begin(), dayList3.end(), compare);
     schedule.append(dayList3);

     QList<Appointment> dayList4 =QList<Appointment>();
     foreach(Appointment a, appointmentList)
     {
         QDate adate = QDate::fromString(a.m_date);

         if(adate==currentDatePlusFour)
         {
             dayList4.append(a);
         }
     }
     std::sort(dayList4.begin(), dayList4.end(), compare);
     schedule.append(dayList4);

     QList<Appointment> dayList5 =QList<Appointment>();
     foreach(Appointment a, appointmentList)
     {
         QDate adate = QDate::fromString(a.m_date);

         if(adate==currentDatePlusFive)
         {
             dayList5.append(a);
         }
     }
     std::sort(dayList5.begin(), dayList5.end(), compare);
     schedule.append(dayList5);

     QList<Appointment> dayList6 =QList<Appointment>();
     foreach(Appointment a, appointmentList)
     {
         QDate adate = QDate::fromString(a.m_date);

         if(adate==currentDatePlusSix)
         {
             dayList6.append(a);
         }
     }
     std::sort(dayList6.begin(), dayList6.end(), compare);
     schedule.append(dayList6);

     QList<Appointment> dayList7 =QList<Appointment>();
     foreach(Appointment a, appointmentList)
     {
         QDate adate = QDate::fromString(a.m_date);

         if(adate==currentDatePlusSeven)
         {
             dayList7.append(a);
         }
     }
     std::sort(dayList7.begin(), dayList7.end(), compare);
     schedule.append(dayList7);

     scheduleListModel = new ScheduleListModel(schedule);
     ui->listViewSchedule->setModel(scheduleListModel);
}

bool DialogUpcomingSchedule::compare(const Appointment &first, const Appointment &second)
{
    QTime firstStarts =QTime::fromString(first.m_startTime);
    QTime secondStarts =QTime::fromString(second.m_startTime);

    int fx=60*60*firstStarts.hour()+60*firstStarts.minute(); //seconds
    int sx=60*60*secondStarts.hour()+60*secondStarts.minute();

    if (fx < sx)
    {
        return true;
    }
    else if (fx > sx)
    {
        return false;
    }
    return false;
}

DialogUpcomingSchedule::~DialogUpcomingSchedule()
{
    delete ui;
}
