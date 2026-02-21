#pragma once

#include <memory>

#include "Core/DataBase/DataBaseRequestData.h"

class LocationManagerBase;
class AddressInputManager;
class ReportsManager;

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

protected:
    std::shared_ptr<LocationManagerBase> m_locationManager;
    std::shared_ptr<database::ReportsGeneratorDBWrapper> m_rgDbWrapper;
    std::unique_ptr<ReportsManager> m_reportsManager;
};

}
