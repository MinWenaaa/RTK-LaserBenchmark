#include <QtWidgets/QApplication>
#include "RTKLaserBenchmark.h"
#include "server.h"

const int port = 3001;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server server;
    server.startServer(port);
    RTKLaserBenchmark w;
    w.show();
    return a.exec();
}
