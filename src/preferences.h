#ifndef PREFERENCES_H
#define PREFERENCES_H

struct Preferences {

    Preferences(
                int playAudio=0,

                int darkCalendar=0,
                int applicationFontSize=20,                
                int lineSpacing=0
            ):

        m_playAudio(playAudio),        
        m_darkCalendar(darkCalendar),
        m_applicationFontSize(applicationFontSize),        
        m_lineSpacing(lineSpacing)

    {
    }

    int m_playAudio;    
    int m_darkCalendar;
    int m_applicationFontSize;    
    int m_lineSpacing;
};


#endif // PREFERENCES_H
