#pragma once

#include "nod/nod.hpp"

namespace location {
    enum class TrackStatus {
        None,
        Start,
        Error
    };

    struct Point {
        double lat = 0.0;
        double lon = 0.0;

        bool operator==(const Point& other) const {
            return (std::abs(lat - other.lat) < std::numeric_limits<double>::epsilon()) &&
                   (std::abs(lon - other.lon) < std::numeric_limits<double>::epsilon());
        }

        bool operator!=(const Point& other) const {
            return !(*this == other);
        }
    };
}

using location_request_cb_t = std::function<void(const std::string& address, const std::optional<location::Point>& location)>;

class LocationManagerBase {
public:
    LocationManagerBase();
    virtual ~LocationManagerBase();

    virtual void startUpdatingLocation() = 0;
    virtual void startTrackingLocation(const location::Point& targetLocation);
    virtual void requestLocationFromAddress(const std::string& address, location_request_cb_t cb) const = 0;

    bool isTrackingLocation() const { return m_currentStatus == location::TrackStatus::Start; };
    location::Point currentLocation() const { return m_location; };

    nod::signal<void(location::TrackStatus status)> locationTrackStatusChanged;
    nod::signal<void(const location::Point& location)> locationChanged;
    nod::signal<void()> targetLocationReached;
    nod::signal<void()> targetLocationAbandoned;

protected:
    virtual bool isLocationInRadius(const location::Point& location, const location::Point& targetLocation, double radius) = 0;

    location::Point m_targetLocation;
    location::Point m_location;
    location::TrackStatus m_currentStatus = location::TrackStatus::None;
};
