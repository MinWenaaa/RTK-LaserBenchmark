#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "client.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Client client;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("client", &client);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GeoMobile-qml", "Main");

    return app.exec();
}
