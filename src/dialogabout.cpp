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

#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    SetAboutMessage();
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::SetAboutMessage()
{
    QString descMsg = "LXQt Organizer is a lightweight organizer for personal information management.";
    QString headerMsg = "LXQt Organizer (v0.004 Alpha1)";
    QString authorMsg = "Author (Lead Developer): Alan Crispin  (aka. crispina)";
    QString lxqtMsg1 =  "LXQt desktop project.";
    QString lxqtLinkMsg ="<a href='https://github.com/lxqt'>LXQt</a>";
    QString license1 ="LXQt Organizer is free software: you can redistribute it and/or modify it under the terms "
                      "of the GNU General Public License as published by the Free Software Foundation.";
    QString
            license2="LXQt Organizer is distributed in the hope that it will be useful, "
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
