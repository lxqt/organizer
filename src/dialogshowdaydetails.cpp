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

#include "dialogshowdaydetails.h"
#include "ui_dialogshowdaydetails.h"

DialogShowDayDetails::DialogShowDayDetails(QWidget *parent,
                                           QDate *theSelectedDate,
                                           DbManager *theDbm) :
    QDialog(parent),
    ui(new Ui::DialogShowDayDetails)
{
    ui->setupUi(this);
    setWindowTitle("Day Events");
    ui->labelSelectedDate->setText(theSelectedDate->toString());
    this->appointmentList= theDbm->getAllAppointments();
    this->theDbm=*theDbm;

    QList<Appointment> dayList =QList<Appointment>();
    foreach(Appointment a, appointmentList)
    {
        QDate adate = QDate::fromString(a.m_date);

        if(adate==*theSelectedDate)
        {
//            qDebug()<<"Appointment: "<<a.m_id<<" "<<a.m_title
//                   <<" "<<a.m_date<<" "<<a.m_category;
            dayList.append(a);

        }
    }

    std::sort(dayList.begin(), dayList.end(), compare);
    dayListModel = new DayListModel(dayList);
    ui->listView->setModel(dayListModel);
}

DialogShowDayDetails::~DialogShowDayDetails()
{
    delete ui;
}

void DialogShowDayDetails::on_listView_doubleClicked(const QModelIndex &index)
{
    int selectedRowIdx=index.row();
    Appointment tmp =dayListModel->getAppointment(selectedRowIdx);
    int dbId =tmp.m_id;
    UpdateAppointment(dbId,selectedRowIdx);

//    int selectedRowIdx=index.row();

//    Appointment tmp =dayListModel->getAppointment(selectedRowIdx);

//    int dbId =tmp.m_id;
//    qDebug()<<"Selected Appointment details...";

//    qDebug()<<"Appointment ID = "<<tmp.m_id;
//    qDebug()<<"Appointment Title = "<<tmp.m_title;
//    qDebug()<<"Appointment Location = "<<tmp.m_location;

//        QMessageBox msgBox;
//        msgBox.setWindowTitle("Delete Appointment");
//        msgBox.setText("Are you sure you want to delete appointment?");
//        msgBox.setStandardButtons(QMessageBox::Yes);
//        msgBox.addButton(QMessageBox::No);
//        msgBox.setDefaultButton(QMessageBox::No);
//        if(msgBox.exec() == QMessageBox::Yes){
//            //remove from model
//            dayListModel->removeAppointment(selectedRowIdx);
//            //remove from database
//            theDbm.deleteAppointmentById(dbId);


//        }else {
//            qDebug()<<"abort delete reminder";
//        }
//        //update reminder listview
//        ui->listView->setModel(dayListModel);



}

void DialogShowDayDetails::UpdateAppointment(int dbID, int selectedRowindex)
{
    Appointment currentAppointment = theDbm.getAppointmentByID(dbID);

    DialogAppointment *appointmentDialog =
            new  DialogAppointment(this,&currentAppointment);
    appointmentDialog->setModal(true);

    if (appointmentDialog->exec() == QDialog::Accepted ) {
        //qDebug()<<"Delete request = "<<appointmentDialog->getDeleteRequested();

        if(appointmentDialog->getDeleteRequested())
        {
            theDbm.deleteAppointmentById(dbID);
            //update reminder listview
            dayListModel->removeAppointment(selectedRowindex);
            // may have to remove reminder from modellist
            ui->listView->setModel(dayListModel);
            return;
        }

        title =appointmentDialog->getTitle();
        location =appointmentDialog->getLocation();
        description= appointmentDialog->getDescription();
        selectedDate=appointmentDialog->getAppointmentDate(); //appointment date
        appointmentStartTime=appointmentDialog->getStartTime();
        appointmentEndTime=appointmentDialog->getEndTime();
        category=appointmentDialog->getCategory();
        isAllDay=appointmentDialog->getAllDay();

        Appointment a;
        a.m_title=title;
        a.m_location=location;
        a.m_description=description;
        a.m_date=selectedDate.toString();
        a.m_startTime=appointmentStartTime.toString();
        a.m_endTime=appointmentEndTime.toString();
        a.m_category=category;
        a.m_isFullDay=isAllDay;
        a.m_isRepeating=0;
        a.m_parentId=0;

        if (theDbm.isOpen())
        {
            bool success =theDbm.updateAppointment(a,dbID);
            qDebug()<<"Apointment update: success ="<<success;

        }

        dayListModel->updateAppointment(a,selectedRowindex);
        ui->listView->setModel(dayListModel);
        ui->listView->update();
    }
}

bool DialogShowDayDetails::compare(const Appointment &first, const Appointment &second)
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
