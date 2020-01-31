#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

DialogPreferences::DialogPreferences(QWidget *parent, Preferences *thePreferences) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);
    setWindowTitle(tr("Font Preferences"));

    ui->spinBoxCalendarFont->setRange(8,38);
    ui->spinBoxCalendarFont->setValue(thePreferences->m_calendarFontSize);
    ui->spinBoxApplicationFont->setRange(8,38);
    ui->spinBoxApplicationFont->setValue(thePreferences->m_applicationFontSize);

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


void DialogPreferences::on_spinBoxApplicationFont_valueChanged(int arg1)
{
    defaultApplicationFontSize=arg1;
}

void DialogPreferences::on_spinBoxCalendarFont_valueChanged(int arg1)
{
    defaultCalendarFontSize=arg1;
}
