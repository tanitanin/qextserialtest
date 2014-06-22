#include "mainwindow.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "serial.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
