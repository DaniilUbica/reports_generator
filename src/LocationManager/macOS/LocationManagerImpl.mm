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

void LocationManagerImpl::startTrackingLocation(const location::Point& targetLocation) {
    LocationManagerBase::startTrackingLocation(targetLocation);

    m_locationTrackingConnect = locationChanged.connect([this](const location::Point& newLocation) {
        if (isLocationInRadius(m_targetLocation, newLocation, 100) && !m_targetLocationReached) {
            m_targetLocationReached = true;
            targetLocationReached();
        }
        else if (m_targetLocationReached) {
            m_targetLocationReached = false;
            targetLocationAbandoned();
        }
    });
}

void LocationManagerImpl::requestLocationFromAddress(const std::string& address, location_request_cb_t cb) const {
    const auto nsAddress = [NSString stringWithUTF8String:address.c_str()];
    const auto geocoder = [[CLGeocoder alloc] init];

    [geocoder geocodeAddressString:nsAddress completionHandler:^(NSArray<CLPlacemark*>* _Nullable placemarks, NSError* _Nullable error) {
        if (error) {
            cb(address, std::nullopt);
            return;
        }

        const auto placemark = [placemarks firstObject];
        if (placemark) {
            const auto coordinate = placemark.location.coordinate;
            const auto location = location::Point{ coordinate.latitude, coordinate.longitude };
            cb(address, { location });
        }
    }];
}

void LocationManagerImpl::setLocationTrackStatus(location::TrackStatus status) {
    if (m_currentStatus != status) {
        m_currentStatus = status;
        locationTrackStatusChanged(m_currentStatus);
    }
}

void LocationManagerImpl::setLocation(double latitude, double longitude) {
    const auto newLocation = location::Point{ latitude, longitude };
    if (m_location != newLocation) {
        m_location = newLocation;
        locationChanged(m_location);
    }
}

bool LocationManagerImpl::isLocationInRadius(const location::Point& location, const location::Point& targetLocation, double radius) {
    const auto loc1 = [[CLLocation alloc] initWithLatitude:location.lat longitude:location.lon];
    const auto loc2 = [[CLLocation alloc] initWithLatitude:targetLocation.lat longitude:targetLocation.lon];

    const auto distance = [loc1 distanceFromLocation:loc2];

    [loc1 release];
    [loc2 release];

    return distance <= radius;
}
