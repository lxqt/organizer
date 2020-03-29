#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

DialogPreferences::DialogPreferences(QWidget *parent, Preferences *thePreferences) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Preferences"));

    ui->spinBoxCalendarFont->setRange(8,38);
    ui->spinBoxCalendarFont->setValue(thePreferences->m_calendarFontSize);
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



}

DialogPreferences::~DialogPreferences()
{
    delete ui;
}

int DialogPreferences::getCalendarFont()
{
   return  defaultCalendarFontSize;

}

int DialogPreferences::getApplicationFont()
{
    return this->defaultApplicationFontSize;
}

int DialogPreferences::getPlayAudio()
{
    return playAudio;
}

QString DialogPreferences::getLocalization()
{
    return locale;
}

int DialogPreferences::getCalendarTheme()
{
    return calendarTheme;
}

int DialogPreferences::getLineSpacing()
{
    return lineSpacing;
}


void DialogPreferences::on_spinBoxApplicationFont_valueChanged(int arg1)
{
    defaultApplicationFontSize=arg1;
}

void DialogPreferences::on_spinBoxCalendarFont_valueChanged(int arg1)
{
    defaultCalendarFontSize=arg1;
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
