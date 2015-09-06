#include <QApplication>
#include "mainwindow.h"

#include "makerlite.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    MakerLite w;
    w.show();

    return a.exec();
}
