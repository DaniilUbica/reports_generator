#pragma once

#include <memory>

#include "Core/DataBase/DataBaseRequestData.h"

class LocationManagerBase;
class AddressInputManager;
class ReportsManager;

namespace location {
    struct Point;
}

namespace rg {

namespace database {
    class ReportsGeneratorDBWrapper;
}

class ConsoleApplication {
public:
    ConsoleApplication(int& argc, char** argv);
    virtual ~ConsoleApplication();

    virtual void onTargetLocationReached();
    virtual void onTargetLocationAbandoned();
    virtual void onLocationFromAddressReceived(const std::string& address, const std::optional<location::Point>& location);

    virtual void startTrackingAddress(const std::string& address);

protected:
    std::shared_ptr<LocationManagerBase> m_locationManager;
    std::shared_ptr<database::ReportsGeneratorDBWrapper> m_rgDbWrapper;
    std::unique_ptr<ReportsManager> m_reportsManager;

    std::string m_trackingAddress;
};

}
