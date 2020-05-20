#include "mainwindow.h"
#include <QApplication>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    umask(0);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
