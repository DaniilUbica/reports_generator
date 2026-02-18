#pragma once

#include <QPoint>
#include <QObject>
#include <QQmlContext>

namespace location {
    enum class TrackStatus {
        None,
        Start,
        Error
    };
}

class LocationManagerBase : public QObject {
    Q_OBJECT
public:
    LocationManagerBase();
    virtual ~LocationManagerBase();

    virtual void startUpdatingLocation();

    bool isTrackingLocation() const { return m_currentStatus == location::TrackStatus::Start; };
    QPointF currentLocation() const { return m_location; };

    Q_SIGNAL void locationTrackStatusChanged(location::TrackStatus status);
    Q_SIGNAL void locationChanged(const QPointF& location);

protected:
    QPointF m_location;
    location::TrackStatus m_currentStatus = location::TrackStatus::None;
};
