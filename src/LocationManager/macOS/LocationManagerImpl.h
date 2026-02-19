#pragma once

#include "LocationManager/LocationManagerBase.h"

class LocationManagerImpl : public LocationManagerBase {
    Q_OBJECT
public:
    LocationManagerImpl();
    ~LocationManagerImpl();

    void startUpdatingLocation() override;
    void startTrackingLocation(const QPointF& targetLocation) override;
    void requestLocationFromAddress(const QString& address, location_request_cb_t cb) const override;

    void setLocationTrackStatus(location::TrackStatus status);
    void setLocation(double latitude, double longitude);

protected:
    bool isLocationInRadius(const QPointF& location, const QPointF& targetLocation, double radius) override;

private:
    class Private;
    std::unique_ptr<Private> d;

    QMetaObject::Connection m_locationTrackingConnect;
};
