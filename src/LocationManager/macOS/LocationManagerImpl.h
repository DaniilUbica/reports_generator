#pragma once

#include "LocationManager/LocationManagerBase.h"

class LocationManagerImpl : public LocationManagerBase {
    Q_OBJECT
public:
    LocationManagerImpl();
    ~LocationManagerImpl();

    void startUpdatingLocation() override;

    void setLocationTrackStatus(location::TrackStatus status);
    void setLocation(double latitude, double longitude);

private:
    class Private;
    std::unique_ptr<Private> d;
};
