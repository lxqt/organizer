#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

DialogPreferences::DialogPreferences(QWidget *parent, Preferences *thePreferences) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);
    SetLableDescriptions();
    setupComboxBoxCalendarLocale();

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
    int index = ui->comboBoxLocale->findText(localeStr);

    if ( index != -1 ) { // -1 for not found must be >0
       ui->comboBoxLocale->setCurrentIndex(index);
    }


}

DialogPreferences::~DialogPreferences()
{
    delete ui;
}

void DialogPreferences::setupComboxBoxCalendarLocale()
{

    ui->comboBoxLocale->addItem(QStringLiteral("English"));
    ui->comboBoxLocale->addItem(QStringLiteral("German"));
    ui->comboBoxLocale->addItem(QStringLiteral("French"));
    ui->comboBoxLocale->addItem(QStringLiteral("Japanese"));

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

QString DialogPreferences::getLocalization()
{
    return localeStr;
}

void DialogPreferences::setTitleTranslation(QString translation)
{
    t_title=translation;
}

void DialogPreferences::setFontSizeTranslation(QString translation)
{
    t_font_size=translation;
}

void DialogPreferences::setLocaleTranslation(QString translation)
{
    t_locale=translation;
}

void DialogPreferences::setPlayAudioTranslation(QString translation)
{
    t_play_audio=translation;
}

void DialogPreferences::setDarkCalendarTranslation(QString translation)
{
    t_dark_calendar=translation;
}

void DialogPreferences::setLineSpacingTranslation(QString translation)
{
    t_line_spacing=translation;
}

void DialogPreferences::SetLableDescriptions()
{
    setWindowTitle(t_title);
    ui->labelFontSize->setText(t_font_size);
    ui->labelLocal->setText(t_locale);
    ui->checkBoxPlayAudio->setText(t_play_audio);
    ui->checkBoxDarkCalendar->setText(t_dark_calendar);
    ui->checkBoxLineSpacing->setText(t_line_spacing);
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
