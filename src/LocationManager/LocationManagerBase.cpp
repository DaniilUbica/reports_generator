#include "LocationManagerBase.h"

LocationManagerBase::LocationManagerBase() {}

LocationManagerBase::~LocationManagerBase() {}

void LocationManagerBase::startTrackingLocation(const QPointF& targetLocation) {
    m_targetLocation = targetLocation;
}
