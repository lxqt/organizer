#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>
#include "preferences.h"

namespace Ui {
class DialogPreferences;
}

class DialogPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPreferences(QWidget *parent = nullptr, Preferences *thePreferences=nullptr);
    ~DialogPreferences();

    int defaultApplicationFontSize=20;
    int getApplicationFont();

    int playAudio=1;
    int isPlayAudio();

    int darkCalendar=0;
    int isDarkCalendar();

    int lineSpacing =0;
    int getLineSpacing();

    QString localeStr =QStringLiteral("English");
    void setupComboxBoxCalendarLocale();
    QString getLocalization();

private slots:
    void on_spinBoxApplicationFont_valueChanged(int arg1);

    void on_checkBoxPlayAudio_stateChanged(int arg1);  

    void on_checkBoxLineSpacing_stateChanged(int arg1);

    void on_comboBoxLocale_activated(const QString &arg1);

    void on_checkBoxDarkCalendar_stateChanged(int arg1);

private:
    Ui::DialogPreferences *ui;
};

#endif // DIALOGPREFERENCES_H
