#include "Application.h"

#include "MapView/MapViewRegister.hpp"
#include "LocationManager/LocationManagerFactory.h"
#include "AddressInputManager/AddressInputManager.h"

#include <QDebug>

using namespace rg;

Application::Application(int& argc, char** argv, int flags) : QGuiApplication(argc, argv, flags) {
    mapview::type_register::registerPlatformType();

    m_locationManager = LocationManagerFactory::requestLocationManager();
    m_locationManager->startUpdatingLocation();

    m_addressInputManager = new AddressInputManager(this);

    connect(m_addressInputManager, &AddressInputManager::addressEntered, this, [this](const QString& address) {
        m_locationManager->requestLocationFromAddress(address, [this](const QString& address, const std::optional<QPointF>& location) {
            if (location.has_value()) {
                QMetaObject::invokeMethod(this, [this, address, location]() {
                    m_locationManager->startTrackingLocation(location.value());
                    Q_EMIT locationRequestCompleted(address, location.value());
                    qDebug() << "target location setted" << location.value();
                }, Qt::QueuedConnection);
            }
        });
    });

    connect(m_locationManager.get(), &LocationManagerBase::targetLocationReached, this, [this]() {
        qDebug() << "target location reached" << m_locationManager->currentLocation();
    });
}

Application::~Application() {}

void Application::setupApplication(QQmlContext* context) {
    context->setContextProperty("rgApplication", this);
    context->setContextProperty("addressInputManager", m_addressInputManager);
}
