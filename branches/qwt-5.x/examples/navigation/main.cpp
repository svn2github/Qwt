#include <qapplication.h>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
#if QT_VERSION < 0x040000
    a.setMainWidget(&mainWindow);
#endif

    mainWindow.resize(800,600);
    mainWindow.show();

    return a.exec();
}

