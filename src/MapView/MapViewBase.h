#pragma once

#include <QQuickItem>

class MapViewBase : public QQuickItem {
    Q_OBJECT
public:
    explicit MapViewBase(QQuickItem* parent = nullptr);
    ~MapViewBase();

    Q_PROPERTY(double latitude MEMBER m_latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_SIGNAL void latitudeChanged();

    Q_PROPERTY(double longitude MEMBER m_longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_SIGNAL void longitudeChanged();

    Q_SIGNAL void clicked();

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
