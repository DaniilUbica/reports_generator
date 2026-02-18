#import "MapViewImpl.h"

#import "MapViewDelegateBridge.h"

#import <MapKit/MapKit.h>

#include <QQuickWindow>

class MapViewImpl::Private final {
public:
    Private(MapViewImpl* parent) : m_parent(parent) {
        mapView = [[MKMapView alloc] init];

        const auto mapViewDelegate = [[MapViewDelegateBridge alloc] init];
        mapViewDelegate.mapView = m_parent;
        mapView.delegate = std::move(mapViewDelegate);
    }

    ~Private() {
        if (mapView) {
            [mapView.delegate release];
            mapView.delegate = nil;

            [mapView removeFromSuperview];
            [mapView release];
            mapView = nullptr;
        }
    }


    MKMapView* mapView = nullptr;

private:
    MapViewImpl* m_parent;
};

MapViewImpl::MapViewImpl(QQuickItem *parent) : MapViewBase(parent) {
    d = std::make_unique<MapViewImpl::Private>(this);
}

MapViewImpl::~MapViewImpl() {}

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

void MapViewImpl::setLatitude(double lat) {
    MapViewBase::setLatitude(lat);

    if (d->mapView && m_latitude != lat) {
        auto center = d->mapView.centerCoordinate;

        center.latitude = lat;
        [d->mapView setCenterCoordinate:center animated:YES];
    }
}

void MapViewImpl::showSelfLocation() {
    [d->mapView setShowsUserLocation:YES];
}

void MapViewImpl::hideSelfLocation() {
    [d->mapView setShowsUserLocation:NO];
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

    const auto parentView = reinterpret_cast<NSView*>(window()->winId());
    [parentView addSubview:d->mapView];
}

void MapViewImpl::geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) {
    MapViewBase::geometryChange(newGeometry, oldGeometry);

    if (d->mapView && isComponentComplete()) {
        const auto frame = NSMakeRect(newGeometry.x(), newGeometry.y(),
                                 newGeometry.width(), newGeometry.height());
        [d->mapView setFrame:frame];
    }
}

double MapViewImpl::altitudeFromZoomLevel(double zoomLevel) {
    return 20000000 / pow(2, zoomLevel);
}
