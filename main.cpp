#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>

#include "MapView/MapViewRegister.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    mapview::type_register::registerPlatformType();

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
