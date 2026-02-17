#pragma once

#include <QQuickItem>

const double MINIMUM_ZOOM_LEVEL = 0.5;
const double MAXIMUM_ZOOM_LEVEL = 20.0;

class MapViewBase : public QQuickItem {
    Q_OBJECT
public:
    explicit MapViewBase(QQuickItem* parent = nullptr);
    ~MapViewBase();

    Q_PROPERTY(double latitude MEMBER m_latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_SIGNAL void latitudeChanged();

    Q_PROPERTY(double longitude MEMBER m_longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_SIGNAL void longitudeChanged();

    Q_PROPERTY(double minimumZoomLevel READ minimumZoomLevel CONSTANT)
    double minimumZoomLevel() const { return MINIMUM_ZOOM_LEVEL; };

    Q_PROPERTY(double maximumZoomLevel READ maximumZoomLevel CONSTANT)
    double maximumZoomLevel() const { return MAXIMUM_ZOOM_LEVEL; };

    Q_INVOKABLE virtual void zoomToMyPosition(double zoomLevel = 5.0);

    virtual void setLongitude(double lon);
    virtual void setLatitude(double lat);

protected:
    virtual void componentComplete() override;
    virtual void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    virtual void zoomToPoint(double latitude, double longitude, double zoomLevel, bool animated);
    virtual double altitudeFromZoomLevel(double zoomLevel);

    double m_latitude = 0.0;
    double m_longitude = 0.0;
};
