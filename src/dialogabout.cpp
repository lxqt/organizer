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


#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    setWindowTitle(tr("About"));
    SetAboutMessage();
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::SetAboutMessage()
{
    QString descMsg = tr("Organizer is a lightweight personal information manager consisting of a calendar and contacts list.");
    descMsg.append(tr(" Data is stored locally in the user account and not online."));
    descMsg.append(tr(" Appointments are added and displayed in the calendar using an entry form allowing the date, "
                      "start and end times, description and category to be set. "
                      "Repeating appointments can be generated. Organizer is a Qt based project."));
    QString headerMsg = tr("Organizer (v0.004)");
    QString authorMsg =tr("Author (Lead Developer): Alan Crispin  (aka. crispina)");
    QString lxqtMsg1 = tr("Desktop project.");
    QString lxqtLinkMsg ="<a href='https://github.com/lxqt'>LXQt</a>";
    QString license1 ="Organizer is free software: you can redistribute it and/or modify it under the terms "
                      "of the GNU General Public License as published by the Free Software Foundation.";
    QString
            license2="Organizer is distributed in the hope that it will be useful, "
                     "but WITHOUT ANY WARRANTY; without even the implied warranty of"
                     " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
                     "See the GNU General Public License for more details.";

    QString msg =descMsg;
        msg.append("\n"); //new line
        msg.append("\n"); //new line
        msg.append(authorMsg);
        msg.append("\n"); //new line
        msg.append("\n"); //new line
        msg.append(lxqtMsg1);
        msg.append("\n"); //new line
        msg.append("\n"); //new line
        msg.append(license1);
        msg.append("\n"); //new line
        msg.append("\n"); //new line
        msg.append(license2);

    ui->labelHeader->setText(headerMsg);
    ui->plainTextEditAbout->setPlainText(msg);
    ui->labelLXQt->setTextFormat(Qt::RichText);
    ui->labelLXQt->setText(lxqtLinkMsg);
}
