#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

DialogPreferences::DialogPreferences(QWidget *parent, Preferences *thePreferences) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);

    ui->spinBoxApplicationFont->setRange(8,38);
    ui->spinBoxApplicationFont->setValue(thePreferences->m_applicationFontSize);

    if(thePreferences->m_playAudio==1)
    {
        ui->checkBoxPlayAudio->setCheckState(Qt::Checked);
    }
    else {
         ui->checkBoxPlayAudio->setCheckState(Qt::Unchecked);
    }

    if(thePreferences->m_lineSpacing==0)
    {
        ui->checkBoxLineSpacing->setCheckState(Qt::Unchecked);
    }
    else {
        ui->checkBoxLineSpacing->setCheckState(Qt::Checked);
    }

    if(thePreferences->m_darkCalendar==0)
    {
        ui->checkBoxDarkCalendar->setCheckState(Qt::Unchecked);
    }
    else {
        ui->checkBoxDarkCalendar->setCheckState(Qt::Checked);
    }

    localeStr=thePreferences->m_localization;
}

DialogPreferences::~DialogPreferences()
{
    delete ui;
}


int DialogPreferences::getApplicationFont()
{
    return this->defaultApplicationFontSize;
}

int DialogPreferences::isPlayAudio()
{
    return playAudio;
}

int DialogPreferences::isDarkCalendar()
{
    return darkCalendar;
}

int DialogPreferences::getLineSpacing()
{
    return lineSpacing;
}

void DialogPreferences::on_spinBoxApplicationFont_valueChanged(int arg1)
{
    defaultApplicationFontSize=arg1;
}



void DialogPreferences::on_checkBoxPlayAudio_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
       playAudio=0;
    }
    else if (arg1==Qt::Checked) {
       playAudio=1;
    }
}



void DialogPreferences::on_checkBoxLineSpacing_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
       lineSpacing=0;
    }
    else if (arg1==Qt::Checked) {
       lineSpacing=1;
    }
}

void DialogPreferences::on_comboBoxLocale_activated(const QString &arg1)
{
    localeStr=arg1;
}

void DialogPreferences::on_checkBoxDarkCalendar_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
    {
       darkCalendar=0;
    }
    else if (arg1==Qt::Checked) {
       darkCalendar=1;
    }
}
