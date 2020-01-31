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
    int defaultCalendarFontSize =12;

    int getCalendarFont();
    int getApplicationFont();



private slots:
    void on_spinBoxApplicationFont_valueChanged(int arg1);

    void on_spinBoxCalendarFont_valueChanged(int arg1);

private:
    Ui::DialogPreferences *ui;
};

#endif // DIALOGPREFERENCES_H
