#pragma once

#include "MapView/MapViewBase.h"

#ifdef __OBJC__
@class MKMapView;
#else
struct objc_object;
typedef struct objc_object MKMapView;
#endif

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

    class Private;
    std::unique_ptr<Private> d;
};
