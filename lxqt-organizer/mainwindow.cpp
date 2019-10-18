#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogsetappointment.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("LXQt Organizer");
    dbc.OpenDatabase();
    dbc.CreateDatabaseTable();
    InitialiseDates();
    model= new QStringListModel(this);
    DisplayAllAppointments();

    //eventDate = ui->calendarWidget->selectedDate();
    //ui->labelSelectedDate->setText(eventDate.toString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitialiseDates()
{
    //initialise event date
    eventDate = ui->calendarWidget->selectedDate();
    day =eventDate.day();
    month=eventDate.month();
    year=eventDate.year();
    ui->labelSelectedDate->setText(eventDate.toString());

    //initialise reminder date
    reminderDays=0;
    reminderDate=eventDate.addDays(-reminderDays);
    remDay=reminderDate.day();
    remMonth=reminderDate.month();
    remYear=reminderDate.year();
}

void MainWindow::DisplayAllAppointments()
{
   model->setStringList(QStringList{});
   QDate tmpDate;
   //QDate tmpReminderDate;
   QSqlQuery query=dbc.SelectAllRecords();
   query.exec();
    //Get data
   while (query.next())
       {
         int idName = query.record().indexOf("ID");
//         int id = query.value(idName).toInt();

          idName = query.record().indexOf("Title");
          QString title = query.value(idName).toString();

          idName = query.record().indexOf("Location");
          QString location = query.value(idName).toString();

           idName = query.record().indexOf("Day");
           int day = query.value(idName).toInt();

           idName = query.record().indexOf("Month");
           int month = query.value(idName).toInt();

           idName = query.record().indexOf("Year");
           int year = query.value(idName).toInt();

           idName = query.record().indexOf("Starts");
           int starttime = query.value(idName).toInt();

//           idName = query.record().indexOf("Ends");
//           int endtime = query.value(idName).toInt();

//           idName = query.record().indexOf("ReminderDay");
//           int remday = query.value(idName).toInt();

//           idName = query.record().indexOf("ReminderMonth");
//           int remmonth = query.value(idName).toInt();

//           idName = query.record().indexOf("ReminderYear");
//           int remyear = query.value(idName).toInt();

//           idName = query.record().indexOf("ReminderTime");
//           int remtime = query.value(idName).toInt();

           tmpDate=*new QDate(year, month,day);

           QString tmpStr = title+ " "
                   +location+" "+tmpDate.toString()
                   +" "+QString::number(starttime)+":00";


           if(model->insertRow(model->rowCount())) {
               QModelIndex index = model->index(model->rowCount() - 1, 0);
               model->setData(index, tmpStr);
           }

           ui->listView->setModel(model);
   }

}

int MainWindow::DayDifferenceBetweenDates(Date dt1, Date dt2)
{
    int n1 = dt1.y*365 + dt1.d;
        for (int i=0; i<dt1.m - 1; i++){
            n1 += monthDays[i];

        }
        n1 += LeapYearNumber(dt1);
        int n2 = dt2.y*365 + dt2.d;
        for (int i=0; i<dt2.m - 1; i++)
        {
            n2 += monthDays[i];

        }
        n2 += LeapYearNumber(dt2);
        return (n2 - n1);

}

int MainWindow::LeapYearNumber(Date d)
{
    int years = d.y;
    if (d.m <= 2)  years--;
    return years/4 - years/100 + years/400;
}

void MainWindow::CheckNotificationsForToday()
{

//    LXQt::Notification *n =nullptr;

//        QDate currentDate = QDate::currentDate();
//        Date todayDate = {currentDate.day(), currentDate.month(), currentDate.year()};

//        Date theEventDate;

//        QSqlQuery query("SELECT * FROM appointment");

//        while (query.next())
//        {
//    //        int idName = query.record().indexOf("ID");
//    //        int id = query.value(idName).toInt();

//            int idName = query.record().indexOf("Date");
//            QString date = query.value(idName).toString();

//            idName = query.record().indexOf("Day");
//            int day = query.value(idName).toInt();

//            idName = query.record().indexOf("Month");
//            int month = query.value(idName).toInt();

//            idName = query.record().indexOf("Year");
//            int year = query.value(idName).toInt();

//            idName = query.record().indexOf("Starts");
//            int starttime = query.value(idName).toInt();

//            idName = query.record().indexOf("Ends");
//            int endtime = query.value(idName).toInt();

//            idName = query.record().indexOf("Description");
//            QString description = query.value(idName).toString();

//            theEventDate ={day, month,year};


//            //int dayDiff =DayDifferenceBetweenDates(todayDate, theEventDate);

//            if (DayDifferenceBetweenDates(todayDate, theEventDate) ==0)
//            {
//                //push notification
//                QString title = "Event today";
//                QString message =" Description: "+description;
//                message.append(" Starts: "+QString::number(starttime)+":00"+" Ends: "+QString::number(endtime)+":00");
//                n = new LXQt::Notification(title);
//                n->setBody(message);
//                n->setTimeout(4000);
//                n->setIcon(QStringLiteral("preferences-desktop-launch-feedback"));
//                n->update();
//            }
//        }
}

void MainWindow::CheckNotificationReminders()
{
    //Todo
}

void MainWindow::on_pushButtonAddAppointment_clicked()
{
    day=eventDate.day();
    month=eventDate.month();
    year=eventDate.year();

    DialogSetAppointment *appointmentDialog = new DialogSetAppointment(this, &eventDate);
    appointmentDialog->setModal(true);

    if (appointmentDialog->exec() == QDialog::Accepted ) {

        title =appointmentDialog->getTitle();
        location =appointmentDialog->getLocation();
        startTime=appointmentDialog->getStartTime();
        endTime=appointmentDialog->getEndTime();

        reminderDate =appointmentDialog->getReminderDate();
        remDay=reminderDate.day();
        remMonth=reminderDate.month();
        remYear=reminderDate.year();
        remTime=appointmentDialog->getReminderTime();


        if (dbc.isOpen())
        {
            dbc.addAppointment(title, location, day, month, year,
                               startTime, endTime,
                               remDay,remMonth,remYear,remTime);
        }
        DisplayAllAppointments();

    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information( this, "LxQt Organizer 0.01",
                              ("Keeping track of appointments and tasks"
                               "\nLXQt Team") );
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    eventDate=date;
    ui->labelSelectedDate->setText(eventDate.toString());
    year =date.year();
    month =date.month();
    day=date.day();
}



void MainWindow::on_actionDelete_All_triggered()
{
    dbc.removeAllAppointments();
    model->setStringList(QStringList{});

}

void MainWindow::on_pushButtonClear_clicked()
{
    model->setStringList(QStringList{});
    //model->removeRows( 0, model.rowCount() )
}

void MainWindow::on_pushButtonShowAll_clicked()
{
    DisplayAllAppointments();
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{

    //currentID =index.row(); //?

}

void MainWindow::on_pushButtonAppointmentToday_clicked()
{
    //Needs an overhaul and rewrite..

    model->setStringList(QStringList{});

    QDate currentDate = QDate::currentDate();
    Date todayDate = {currentDate.day(), currentDate.month(), currentDate.year()};

    Date theEventDate;

       QSqlQuery query("SELECT * FROM appointment");

       while (query.next())
       {

           int idName = query.record().indexOf("Title");
           QString title = query.value(idName).toString();

         idName = query.record().indexOf("Location");
          QString location = query.value(idName).toString();

           idName = query.record().indexOf("Day");
           int day = query.value(idName).toInt();

           idName = query.record().indexOf("Month");
           int month = query.value(idName).toInt();

           idName = query.record().indexOf("Year");
           int year = query.value(idName).toInt();

           idName = query.record().indexOf("Starts");
           int starttime = query.value(idName).toInt();

//           idName = query.record().indexOf("Ends");
//           int endtime = query.value(idName).toInt();

           theEventDate ={day, month,year};
           QDate tmpDate=*new QDate(year, month,day);


           if (DayDifferenceBetweenDates(todayDate, theEventDate) ==0)
           {
               QString tmpStr = title+ " "
                       +location+" "+tmpDate.toString()
                       +" "+QString::number(starttime)+":00";

               if(model->insertRow(model->rowCount())) {
                   QModelIndex index = model->index(model->rowCount() - 1, 0);
                   model->setData(index, tmpStr);
               }

               ui->listView->setModel(model);
           }
       }
}
