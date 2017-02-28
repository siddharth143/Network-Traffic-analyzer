#include <QtGui/QApplication>
#include "trafficanalyzerdialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TADialog  w;

    w.show();

    return a.exec();
}
