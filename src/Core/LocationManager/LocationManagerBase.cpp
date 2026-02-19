#include "LocationManagerBase.h"

LocationManagerBase::LocationManagerBase() {}

LocationManagerBase::~LocationManagerBase() {}

void LocationManagerBase::startTrackingLocation(const location::Point& targetLocation) {
    m_targetLocation = targetLocation;
}
