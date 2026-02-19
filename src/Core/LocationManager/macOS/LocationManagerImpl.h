#pragma once

#include "Core/LocationManager/LocationManagerBase.h"

class LocationManagerImpl : public LocationManagerBase {
public:
    LocationManagerImpl();
    ~LocationManagerImpl();

    void startUpdatingLocation() override;
    void startTrackingLocation(const location::Point& targetLocation) override;
    void requestLocationFromAddress(const std::string& address, location_request_cb_t cb) const override;

    void setLocationTrackStatus(location::TrackStatus status);
    void setLocation(double latitude, double longitude);

protected:
    bool isLocationInRadius(const location::Point& location, const location::Point& targetLocation, double radius) override;

private:
    class Private;
    std::unique_ptr<Private> d;

    nod::scoped_connection m_locationTrackingConnect;
    bool m_targetLocationReached = false;
};
