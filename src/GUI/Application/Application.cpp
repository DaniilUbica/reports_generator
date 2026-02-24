#include "Application.h"

#include "GUI/MapView/MapViewRegister.hpp"
#include "GUI/AddressInputManager/AddressInputManager.h"
#include "Core/LocationManager/LocationManagerFactory.h"

#include <QDebug>

using namespace rg;

Application::Application(int& argc, char** argv, int flags) :
    QGuiApplication(argc, argv, flags),
    ConsoleApplication(argc, argv)
{
    mapview::type_register::registerPlatformType();

    m_addressInputManager = new AddressInputManager(this);

    connect(m_addressInputManager, &AddressInputManager::addressEntered, this, [this](const QString& address) {
        startTrackingAddress(address.toStdString());
    });
}

Application::~Application() {}

void Application::setupApplication(QQmlContext* context) {
    context->setContextProperty("rgApplication", this);
    context->setContextProperty("addressInputManager", m_addressInputManager);
}

void Application::onTargetLocationReached() {
    ConsoleApplication::onTargetLocationReached();

    qDebug() << "target location reached";
}

void Application::onTargetLocationAbandoned() {
    ConsoleApplication::onTargetLocationAbandoned();

    qDebug() << "target location abandoned";
}

void Application::onLocationFromAddressReceived(const std::string& address, const std::optional<location::Point>& location) {
    if (location.has_value()) {
        QMetaObject::invokeMethod(this, [this, address, location]() {
            m_locationManager->startTrackingLocation(location.value());
            Q_EMIT locationRequestCompleted(QString::fromStdString(address), { location->lat, location->lon });

            qDebug() << "target location setted" << QPointF{ location->lat, location->lon };
        }, Qt::QueuedConnection);
    }
}

void Application::startTrackingAddress(const std::string& address) {
    ConsoleApplication::startTrackingAddress(address);

    setTrackingAddress(QString::fromStdString(address));
}

void Application::setTrackingAddress(const QString& address) {
    if (m_trackingAddress != address.toStdString()) {
        m_trackingAddress = address.toStdString();
        Q_EMIT trackingAddressChanged(address);
    }
}
