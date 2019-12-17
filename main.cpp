#include "mainwindow.h"
#include "atomicstate.h"

Q_DECLARE_METATYPE(QString)

#include <QApplication>
#include <curlpp/cURLpp.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("DistoRt");

    qRegisterMetaType<QString>();
    curlpp::initialize();
    AtomicState* atomicState = AtomicState::getInstance();

    MainWindow w;
    w.show();

    int r = a.exec();
    curlpp::terminate();
    delete atomicState;

    return r;
}
