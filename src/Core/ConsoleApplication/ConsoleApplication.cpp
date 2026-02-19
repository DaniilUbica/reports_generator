#include "ConsoleApplication.h"

#include "Core/LocationManager/LocationManagerFactory.h"
#include "Core/ReportsManager/ReportsManager.h"

using namespace rg;

ConsoleApplication::ConsoleApplication(int& argc, char** argv) {
    m_locationManager = LocationManagerFactory::requestLocationManager();
    m_reportsManager = std::make_unique<ReportsManager>();

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
    auto options = ReportOptions { true };
    m_reportsManager->startNewReport(std::move(options));
}

void ConsoleApplication::onTargetLocationAbandoned() {
    m_reportsManager->endCurrentReport();
}
