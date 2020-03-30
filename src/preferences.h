#ifndef PREFERENCES_H
#define PREFERENCES_H
#include <QString>
struct Preferences {

    Preferences(
                int playAudio=1,
                const QString& locale= QString(),
                int calendarTheme=0,
                int applicationFontSize=20,
                int calendarFontSize =12,
                int lineSpacing=0
            ):

        m_playAudio(playAudio),
        m_localization(locale),
        m_calendarTheme(calendarTheme),
        m_applicationFontSize(applicationFontSize),
        m_calendarFontSize(calendarFontSize),
        m_lineSpacing(lineSpacing)

    {
    }

    int m_playAudio;
    QString m_localization;
    int m_calendarTheme;
    int m_applicationFontSize;
    int m_calendarFontSize;
    int m_lineSpacing;
};


#endif // PREFERENCES_H
