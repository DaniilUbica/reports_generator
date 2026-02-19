#ifdef __APPLE__
    #include "GUI/MapView/macOS/MapViewImpl.h"
#else
    #include "MapView/MapViewBase.h"
#endif

namespace mapview {
namespace type_register {

    static void registerPlatformType() {
        #ifdef __APPLE__
            qmlRegisterType<MapViewImpl>("reports.generator.types", 1, 0, "MapView");
        #else
            qmlRegisterType<MapViewBase>("reports.generator.types", 1, 0, "MapView");
        #endif
    }

}
}
