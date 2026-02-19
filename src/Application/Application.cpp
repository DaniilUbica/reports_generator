#include "Application.h"

#include "MapView/MapViewRegister.hpp"
#include "LocationManager/LocationManagerFactory.h"
#include "AddressInputManager/AddressInputManager.h"

#include <QDebug>

using namespace rg;

Application::Application(int& argc, char** argv, int flags) :
    QGuiApplication(argc, argv, flags),
    ConsoleApplication(argc, argv)
{
    mapview::type_register::registerPlatformType();

    m_addressInputManager = new AddressInputManager(this);

    connect(m_addressInputManager, &AddressInputManager::addressEntered, this, [this](const QString& address) {
        m_locationManager->requestLocationFromAddress(address.toStdString(), [this](const std::string& address, const std::optional<location::Point>& location) {
            if (location.has_value()) {
                QMetaObject::invokeMethod(this, [this, address, location]() {
                    m_locationManager->startTrackingLocation(location.value());
                    Q_EMIT locationRequestCompleted(QString::fromStdString(address), { location->lat, location->lon });

                    qDebug() << "target location setted" << QPointF{ location->lat, location->lon };
                }, Qt::QueuedConnection);
            }
        });
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
