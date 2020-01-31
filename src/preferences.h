#ifndef PREFERENCES_H
#define PREFERENCES_H

struct Preferences {

    Preferences(int id=0,
                int applicationFontSize=20,
                int calendarFontSize =12
            ):
        m_id(id),
        m_applicationFontSize(applicationFontSize),
        m_calendarFontSize(calendarFontSize)

    {
    }
    int m_id;
    int m_applicationFontSize;
    int m_calendarFontSize;
};


#endif // PREFERENCES_H
