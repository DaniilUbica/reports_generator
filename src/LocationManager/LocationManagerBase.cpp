#include "LocationManagerBase.h"

LocationManagerBase::LocationManagerBase() {}

LocationManagerBase::~LocationManagerBase() {}

void LocationManagerBase::startUpdatingLocation() {}

void LocationManagerBase::requestLocationFromAddress(const QString& address, location_request_cb_t cb) const {}
