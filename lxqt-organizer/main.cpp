#include "mainwindow.h"
#include <QApplication>
#include "systemtimer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Create a SystemTimer instance
    SystemTimer timer;
    return a.exec();
}
