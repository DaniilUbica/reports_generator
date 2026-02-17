#import "LocationManagerDelegateBridge.h"
#import "MapViewImpl.h"

#import <MapKit/MapKit.h>

@implementation LocationManagerDelegateBridge

- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status {
    switch (status) {
        case kCLAuthorizationStatusAuthorizedAlways:
            if (self.authStatusAuthAlwaysCB) {
                self.authStatusAuthAlwaysCB();
            }
            break;
        case kCLAuthorizationStatusDenied:
        case kCLAuthorizationStatusRestricted:
        default:
            break;
    }
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations {
    CLLocation* location = [locations lastObject];
    if (location && self.locationChangedCB) {
        self.locationChangedCB(location);
    }
}

@end
