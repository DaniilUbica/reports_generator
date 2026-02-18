#include "LocationManagerImpl.h"

#include "LocationManagerDelegateBridge.h"

class LocationManagerImpl::Private final {
public:
    Private(LocationManagerImpl* parent) : m_parent(parent) {
        locationManager = [[CLLocationManager alloc] init];

        const auto locationManagerDelegate = [[LocationManagerDelegateBridge alloc] init];
        locationManagerDelegate.authStatusAuthAlwaysCB = ^{
            m_parent->setLocationTrackStatus(location::TrackStatus::Start);
        };
        locationManagerDelegate.authStatusErrorCB = ^ {
            m_parent->setLocationTrackStatus(location::TrackStatus::Error);
        };
        locationManagerDelegate.locationChangedCB = ^(CLLocation* location) {
            m_parent->setLocation(location.coordinate.latitude, location.coordinate.longitude);
        };

        locationManager.delegate = std::move(locationManagerDelegate);

        [locationManager requestAlwaysAuthorization];
    }

    ~Private() {
        if (locationManager) {
            [locationManager.delegate release];
            locationManager.delegate = nil;

            [locationManager release];
            locationManager = nullptr;
        }
    }

    CLLocationManager* locationManager = nullptr;
    LocationManagerDelegateBridge* qqq;

private:
    LocationManagerImpl* m_parent;
};

LocationManagerImpl::LocationManagerImpl() {
    d = std::make_unique<LocationManagerImpl::Private>(this);
}

LocationManagerImpl::~LocationManagerImpl() {}

void LocationManagerImpl::startUpdatingLocation() {
    [d->locationManager startUpdatingLocation];
}

void LocationManagerImpl::setLocationTrackStatus(location::TrackStatus status) {
    if (m_currentStatus != status) {
        m_currentStatus = status;
        Q_EMIT locationTrackStatusChanged(m_currentStatus);
    }
}

void LocationManagerImpl::setLocation(double latitude, double longitude) {
    const auto newLocation = QPointF(latitude, longitude);
    if (m_location != newLocation) {
        m_location = newLocation;
        Q_EMIT locationChanged(m_location);
    }
}
