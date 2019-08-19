#include "mainwindow.h"

#include <QApplication>
#include <curlpp/cURLpp.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("DistoRt");

    curlpp::initialize();

    MainWindow w;
    w.show();

    int r = a.exec();
    curlpp::terminate();
    return r;
}
