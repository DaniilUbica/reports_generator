#include "LocationManagerFactory.h"

namespace LocationManagerFactory {

static std::weak_ptr<LocationManagerBase> s_instance;

std::shared_ptr<LocationManagerBase> requestLocationManager() {
    if (const auto sp = s_instance.lock()) {
        return sp;
    }

    #ifdef __APPLE__
        const auto sp = std::make_shared<LocationManagerImpl>();
    #else
        const auto sp = std::make_shared<LocationManagerBase>();
    #endif

    s_instance = sp;
    return sp;
}

}
