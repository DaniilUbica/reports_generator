#import "MapViewImpl.h"

#import "MapViewDelegateBridge.h"
#import "LocationManagerDelegateBridge.h"

#import <MapKit/MapKit.h>

#include <QQuickWindow>

class MapViewImpl::Private final {
public:
    Private(MapViewImpl* parent) : m_parent(parent) {
        mapView = [[MKMapView alloc] init];
        locationManager = [[CLLocationManager alloc] init];

        const auto mapViewDelegate = [[MapViewDelegateBridge alloc] init];
        mapViewDelegate.mapView = m_parent;
        mapView.delegate = std::move(mapViewDelegate);

        const auto locationManagerDelegate = [[LocationManagerDelegateBridge alloc] init];
        locationManagerDelegate.mapView = m_parent;
        locationManagerDelegate.authStatusAuthAlwaysCB = ^{
            [mapView setShowsUserLocation:YES];
        };
        locationManager.delegate = std::move(locationManagerDelegate);

        [locationManager requestAlwaysAuthorization];
    }

    ~Private() {
        if (mapView) {
            [mapView.delegate release];
            mapView.delegate = nil;

            [mapView removeFromSuperview];
            [mapView release];
            mapView = nullptr;
        }

        if (locationManager) {
            [locationManager.delegate release];
            locationManager.delegate = nil;

            [locationManager release];
            locationManager = nullptr;
        }
    }


    MKMapView* mapView = nullptr;
    CLLocationManager* locationManager = nullptr;

private:
    MapViewImpl* m_parent;
};

MapViewImpl::MapViewImpl(QQuickItem *parent) : MapViewBase(parent) {
    d = std::make_unique<MapViewImpl::Private>(this);

    connect(this, &MapViewImpl::clicked, this, [this]() {
        [d->locationManager startUpdatingLocation];

        if (d->locationManager.location != nil) {
            zoomToPoint(d->locationManager.location.coordinate.latitude, d->locationManager.location.coordinate.longitude, 5, true);
        }
    });
}

MapViewImpl::~MapViewImpl() {}

void MapViewImpl::setLatitude(double lat) {
    MapViewBase::setLatitude(lat);

    if (d->mapView && m_latitude != lat) {
        auto center = d->mapView.centerCoordinate;

        center.latitude = lat;
        [d->mapView setCenterCoordinate:center animated:YES];
    }
}

void MapViewImpl::setLongitude(double lon) {
    MapViewBase::setLongitude(lon);

    if (d->mapView && m_longitude != lon) {
        auto center = d->mapView.centerCoordinate;

        center.longitude = lon;
        [d->mapView setCenterCoordinate:center animated:YES];
    }
}

void MapViewImpl::componentComplete() {
    MapViewBase::componentComplete();

    const auto center = CLLocationCoordinate2DMake(m_latitude, m_longitude);
    [d->mapView setCenterCoordinate:center animated:NO];

    const auto parentView = reinterpret_cast<NSView*>(window()->winId());
    [parentView addSubview:d->mapView];
}

void MapViewImpl::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    MapViewBase::geometryChange(newGeometry, oldGeometry);

    if (d->mapView && isComponentComplete()) {
        const auto frame = NSMakeRect(newGeometry.x(), newGeometry.y(),
                                 newGeometry.width(), newGeometry.height());
        [d->mapView setFrame:frame];
    }
}

void MapViewImpl::zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated) {
    if (!d->mapView || !isComponentComplete()) {
        return;
    }

    const auto center = CLLocationCoordinate2DMake(latitude, longitude);
    const auto altitude = altitudeFromZoomLevel(zoomLevel);

    const auto camera = [MKMapCamera cameraLookingAtCenterCoordinate:center
                                                      fromEyeCoordinate:center
                                                            eyeAltitude:altitude];

    [d->mapView setCamera:camera animated:animated];
}

double MapViewImpl::altitudeFromZoomLevel(double zoomLevel) {
    return 20000000 / pow(2, zoomLevel);
}
