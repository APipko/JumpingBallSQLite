#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>
#include "ballstate.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<BallState>("BallState");
    MainWindow w;
    w.show();

    return a.exec();
}
