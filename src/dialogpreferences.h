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


    //Translations
    QString t_title=QStringLiteral("Preferences");
    QString t_font_size=QStringLiteral("Font Size ");
    QString t_locale=QStringLiteral("Locale ");
    QString t_play_audio=QStringLiteral("Play Audio");
    QString t_dark_calendar=QStringLiteral("Dark Calendar");
    QString t_line_spacing=QStringLiteral("Line Spacing");

    void setTitleTranslation(QString translation);
    void setFontSizeTranslation(QString translation);
    void setLocaleTranslation(QString translation);
    void setPlayAudioTranslation(QString translation);
    void setDarkCalendarTranslation(QString translation);
    void setLineSpacingTranslation(QString translation);

    void SetLableDescriptions();

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
