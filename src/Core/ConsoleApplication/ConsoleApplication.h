#pragma once

#include <memory>

class LocationManagerBase;
class AddressInputManager;
class ReportsManager;

namespace rg {

class ConsoleApplication {
public:
    ConsoleApplication(int& argc, char** argv);
    virtual ~ConsoleApplication();

    virtual void onTargetLocationReached();
    virtual void onTargetLocationAbandoned();

protected:
    std::shared_ptr<LocationManagerBase> m_locationManager;
    std::unique_ptr<ReportsManager> m_reportsManager;
};

}
