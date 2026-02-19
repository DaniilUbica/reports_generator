#pragma once

#ifdef __APPLE__
    #include "macOS/LocationManagerImpl.h"
#else
    #include "LocationManagerBase.h"
#endif

namespace LocationManagerFactory {

std::shared_ptr<LocationManagerBase> requestLocationManager();

}
