#pragma once

#include "MapView/MapViewBase.h"

class MapViewImpl : public MapViewBase {
    Q_OBJECT
public:
    explicit MapViewImpl(QQuickItem *parent = nullptr);
    ~MapViewImpl();

    virtual void setLongitude(double lon) override;
    virtual void setLatitude(double lat) override;

protected:
    void componentComplete() override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    void zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated) override;
    double altitudeFromZoomLevel(double zoomLevel) override;

    void* m_mapView = nullptr;
};
