#import "MapViewBase.h"

#include "Core/LocationManager/LocationManagerFactory.h"

#include <QQuickWindow>

MapViewBase::MapViewBase(QQuickItem *parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);

    m_locationManager = LocationManagerFactory::requestLocationManager();
    m_locationManager->locationTrackStatusChanged.connect([this](location::TrackStatus status) {
        QMetaObject::invokeMethod(this, [this, status]() {
            if (status == location::TrackStatus::Start) {
                showSelfLocation();
            }
            else if (status == location::TrackStatus::Error) {
                hideSelfLocation();
            }
        }, Qt::QueuedConnection);
    });

    if (m_locationManager->isTrackingLocation()) {
        showSelfLocation();
        zoomToMyLocation(5.0, true);
    }
}

MapViewBase::~MapViewBase() {}

void MapViewBase::zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated) {}

void MapViewBase::zoomToMyLocation(double zoomLevel, bool animated) {
    const auto location = m_locationManager->currentLocation();
    zoomToPoint(location.lat, location.lon, zoomLevel, animated);
}

void MapViewBase::setLatitude(double lat) {
    if (qFuzzyCompare(m_center.x(), lat)) {
        return;
    }

    m_center.setX(lat);
    Q_EMIT centerChanged(m_center);
}

void MapViewBase::setLongitude(double lon) {
    if (qFuzzyCompare(m_center.y(), lon)){
        return;
    }

    m_center.setY(lon);
    Q_EMIT centerChanged(m_center);
}

void MapViewBase::showSelfLocation() {}

void MapViewBase::hideSelfLocation() {}

void MapViewBase::componentComplete() {
    QQuickItem::componentComplete();
}

void MapViewBase::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
}

double MapViewBase::altitudeFromZoomLevel(double zoomLevel) {
    return 0.0;
}
