#pragma once

#include <QPoint>
#include <QObject>
#include <QQmlContext>

#include <functional>

namespace location {
    enum class TrackStatus {
        None,
        Start,
        Error
    };
}

using location_request_cb_t = std::function<void(const QString& address, const std::optional<QPointF>& location)>;

class LocationManagerBase : public QObject {
    Q_OBJECT
public:
    LocationManagerBase();
    virtual ~LocationManagerBase();

    virtual void startUpdatingLocation();

    bool isTrackingLocation() const { return m_currentStatus == location::TrackStatus::Start; };
    QPointF currentLocation() const { return m_location; };

    virtual void requestLocationFromAddress(const QString& address, location_request_cb_t cb) const;

    Q_SIGNAL void locationTrackStatusChanged(location::TrackStatus status);
    Q_SIGNAL void locationChanged(const QPointF& location);

protected:
    QPointF m_location;
    location::TrackStatus m_currentStatus = location::TrackStatus::None;
};
