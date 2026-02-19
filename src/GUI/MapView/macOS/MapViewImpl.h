#pragma once

#include "GUI/MapView/MapViewBase.h"

class MapViewImpl : public MapViewBase {
    Q_OBJECT
public:
    explicit MapViewImpl(QQuickItem *parent = nullptr);
    ~MapViewImpl();

    void zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated) override;

    void setLongitude(double lon) override;
    void setLatitude(double lat) override;

    void showSelfLocation() override;
    void hideSelfLocation() override;

protected:
    void componentComplete() override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    double altitudeFromZoomLevel(double zoomLevel) override;

    class Private;
    std::unique_ptr<Private> d;
};
