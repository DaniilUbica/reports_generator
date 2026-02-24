#include "ConsoleApplication.h"

#include "Core/LocationManager/LocationManagerFactory.h"
#include "Core/ReportsManager/ReportsManager.h"

#include "Core/DataBase/Wrappers/ReportsGeneratorDBWrapper.h"

using namespace rg;

ConsoleApplication::ConsoleApplication(int& argc, char** argv) {
    m_locationManager = LocationManagerFactory::requestLocationManager();
    m_reportsManager = std::make_unique<ReportsManager>();
    m_rgDbWrapper = database::ReportsGeneratorDBWrapper::instance();

    m_locationManager->startUpdatingLocation();

    m_locationManager->targetLocationReached.connect([this]() {
        onTargetLocationReached();
    });
    m_locationManager->targetLocationAbandoned.connect([this]() {
        onTargetLocationAbandoned();
    });

    const auto trackingAddress = m_rgDbWrapper->getValue<std::string>(database::TRACKING_ADDRESS_FIELD_NAME);
    if (trackingAddress.has_value()) {
        startTrackingAddress(trackingAddress.value());
    }
}

ConsoleApplication::~ConsoleApplication() {}

void ConsoleApplication::onTargetLocationReached() {
    auto options = ReportOptions { true };
    m_reportsManager->startNewReport(std::move(options));
}

void ConsoleApplication::onTargetLocationAbandoned() {
    m_reportsManager->endCurrentReport();
}

void ConsoleApplication::onLocationFromAddressReceived(const std::string& address, const std::optional<location::Point>& location) {}

void ConsoleApplication::startTrackingAddress(const std::string& address) {
    if (m_trackingAddress != address) {
        m_trackingAddress = address;
        assert(m_rgDbWrapper->setValue(database::TRACKING_ADDRESS_FIELD_NAME, m_trackingAddress));

        m_locationManager->requestLocationFromAddress(address, [this](const std::string& address, const std::optional<location::Point>& location) {
            onLocationFromAddressReceived(address, location);
        });
    }
}
