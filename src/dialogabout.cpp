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

    QString strap=tr("Organizer is a Qt lightweight personal information manager.");

    QString keyMsg="";
    keyMsg.append(tr(" Calendar Keyboard shortcuts\n"));
    keyMsg.append(" ---------------------------------------\n");
    keyMsg.append(tr(" Up arrow = next month\n"));
    keyMsg.append(tr(" Down arrow = previous month\n"));
    keyMsg.append(tr(" Space bar = (back to) today\n\n"));

    keyMsg.append(tr(" Single click calendar date = show appointments\n"));
    keyMsg.append(tr(" Double click calendar date = new appointment\n\n"));

    keyMsg.append(tr(" key_c = new contact\n"));

    QString headerMsg = tr("Organizer (v0.5)");
    QString authorMsg =tr("Author (Lead Developer): Alan Crispin  (aka. crispina)");
    QString lxqtMsg1 = tr("Organizer is maintained by the LXQt project but can be "
                          "used independently from this desktop environment."
                          );
    QString lxqtLinkMsg ="<a href='https://github.com/lxqt'>LXQt</a>";
    QString license1 ="Organizer is free software: you can redistribute it and/or modify it under the terms "
                      "of the GNU General Public License as published by the Free Software Foundation.";
    QString
            license2="Organizer is distributed in the hope that it will be useful, "
                     "but WITHOUT ANY WARRANTY; without even the implied warranty of"
                     " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
                     "See the GNU General Public License for more details.";

    QString msg =strap;
        msg.append("\n"); //new line
        msg.append("\n"); //new line
        msg.append(authorMsg);
        msg.append("\n"); //new line
        msg.append("\n"); //new line
        msg.append(keyMsg);
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
