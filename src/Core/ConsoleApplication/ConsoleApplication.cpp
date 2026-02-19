#include "ConsoleApplication.h"

#include "Core/LocationManager/LocationManagerFactory.h"

using namespace rg;

ConsoleApplication::ConsoleApplication(int& argc, char** argv) {
    m_locationManager = LocationManagerFactory::requestLocationManager();
    m_locationManager->startUpdatingLocation();

    m_locationManager->targetLocationReached.connect([this]() {
        onTargetLocationReached();
    });
    m_locationManager->targetLocationAbandoned.connect([this]() {
        onTargetLocationAbandoned();
    });
}

ConsoleApplication::~ConsoleApplication() {}

void ConsoleApplication::onTargetLocationReached() {

}

void ConsoleApplication::onTargetLocationAbandoned() {

}
