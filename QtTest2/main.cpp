#include "mainwindow.h"
#include <QApplication>
#include "simplerender.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();

    SimpleRender w;
    w.show();

    return a.exec();
}
