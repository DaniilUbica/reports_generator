#import "MapViewBase.h"

#include "LocationManager/LocationManagerFactory.h"

#include <QQuickWindow>

MapViewBase::MapViewBase(QQuickItem *parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);

    m_locationManager = LocationManagerFactory::requestLocationManager();
    connect(m_locationManager.get(), &LocationManagerBase::locationTrackStatusChanged, this, [this](location::TrackStatus status) {
        if (status == location::TrackStatus::Start) {
            showSelfLocation();
        }
        else if (status == location::TrackStatus::Error) {
            hideSelfLocation();
        }
    });

    if (m_locationManager->isTrackingLocation()) {
        showSelfLocation();
    }
}

MapViewBase::~MapViewBase() {}

void MapViewBase::zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated) {}

void MapViewBase::zoomToMyLocation(double zoomLevel, bool animated) {
    const auto location = m_locationManager->currentLocation();
    zoomToPoint(location.x(), location.y(), zoomLevel, animated);
}

void MapViewBase::setLatitude(double lat) {
    if (qFuzzyCompare(m_latitude, lat)) {
        return;
    }

    m_latitude = lat;
    Q_EMIT latitudeChanged();
}

void MapViewBase::setLongitude(double lon) {
    if (qFuzzyCompare(m_longitude, lon)){
        return;
    }

    m_longitude = lon;
    Q_EMIT longitudeChanged();
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
