#import <CoreLocation/CoreLocation.h>

#ifdef __cplusplus
class MapViewImpl;
#endif

@interface LocationManagerDelegateBridge : NSObject<CLLocationManagerDelegate>
@property (assign) MapViewImpl* mapView;
@property (copy) void (^authStatusAuthAlwaysCB)(void);
@property (copy) void (^locationChangedCB)(CLLocation*);
@end
