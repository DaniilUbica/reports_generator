#import "MapViewBase.h"

#include <QQuickWindow>

MapViewBase::MapViewBase(QQuickItem *parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
}

MapViewBase::~MapViewBase() {}

void MapViewBase::zoomToMyPosition(double zoomLevel) {}

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

void MapViewBase::componentComplete() {
    QQuickItem::componentComplete();
}

void MapViewBase::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
}

void MapViewBase::zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated) {}

double MapViewBase::altitudeFromZoomLevel(double zoomLevel) {
    return 0.0;
}
