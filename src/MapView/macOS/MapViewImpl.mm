#import "MapViewImpl.h"

#import "MapViewDelegateBridge.h"

#import <MapKit/MapKit.h>

#include <QQuickWindow>

MapViewImpl::MapViewImpl(QQuickItem *parent) : MapViewBase(parent) {
    connect(this, &MapViewImpl::clicked, this, [this](){
        zoomToPoint(12, 12, 2, true);
    });
}

MapViewImpl::~MapViewImpl() {
    if (m_mapView) {
        const auto mapView = (__bridge MKMapView*)m_mapView;
        const auto delegate = (MapViewDelegateBridge*)mapView.delegate;

        mapView.delegate = nil;
        delegate.mapView = nullptr;

        [delegate release];
        [mapView removeFromSuperview];
        [mapView release];
    }
}

void MapViewImpl::setLatitude(double lat) {
    MapViewBase::setLatitude(lat);

    if (m_mapView && m_latitude != lat) {
        const auto mapView = (__bridge MKMapView*)m_mapView;
        auto center = mapView.centerCoordinate;

        center.latitude = lat;
        [mapView setCenterCoordinate:center animated:YES];
    }
}

void MapViewImpl::setLongitude(double lon) {
    MapViewBase::setLongitude(lon);

    if (m_mapView && m_longitude != lon) {
        const auto mapView = (__bridge MKMapView*)m_mapView;
        auto center = mapView.centerCoordinate;

        center.longitude = lon;
        [mapView setCenterCoordinate:center animated:YES];
    }
}

void MapViewImpl::componentComplete() {
    MapViewBase::componentComplete();

    const auto mapView = [[MKMapView alloc] init];
    const auto delegate = [[MapViewDelegateBridge alloc] init];

    delegate.mapView = this;
    mapView.delegate = delegate;

    const auto center = CLLocationCoordinate2DMake(m_latitude, m_longitude);
    [mapView setCenterCoordinate:center animated:NO];

    const auto parentView = reinterpret_cast<NSView*>(window()->winId());
    [parentView addSubview:mapView];

    m_mapView = (void*)CFBridgingRetain(mapView);
}

void MapViewImpl::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    MapViewBase::geometryChange(newGeometry, oldGeometry);

    if (m_mapView && isComponentComplete()) {
        const auto mapView = (__bridge MKMapView*)m_mapView;
        const auto frame = NSMakeRect(newGeometry.x(), newGeometry.y(),
                                 newGeometry.width(), newGeometry.height());
        [mapView setFrame:frame];
    }
}

void MapViewImpl::zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated) {
    if (!m_mapView || !isComponentComplete()) {
        return;
    }

    const auto mapView = (__bridge MKMapView *)m_mapView;
    const auto delegate = (MapViewDelegateBridge*)mapView.delegate;
    const auto center = CLLocationCoordinate2DMake(latitude, longitude);
    const auto altitude = altitudeFromZoomLevel(zoomLevel);

    const auto camera = [MKMapCamera cameraLookingAtCenterCoordinate:center
                                                      fromEyeCoordinate:center
                                                            eyeAltitude:altitude];

    [mapView setCamera:camera animated:animated];
}

double MapViewImpl::altitudeFromZoomLevel(double zoomLevel) {
    return 20000000 / pow(2, zoomLevel);
}
