#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("RovshanBaghirov");
    QCoreApplication::setOrganizationDomain("tanzil.net");
    QCoreApplication::setApplicationName("MemorizeQuran");

    MainWindow w;
    w.show();

    return a.exec();
}
