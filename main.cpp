#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>

#include "MapView/MapViewRegister.hpp"
#include "LocationManager/LocationManagerFactory.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    mapview::type_register::registerPlatformType();
    const auto locationManager = LocationManagerFactory::requestLocationManager();
    locationManager->startUpdatingLocation();

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/main.qml"));

    return app.exec();
}
