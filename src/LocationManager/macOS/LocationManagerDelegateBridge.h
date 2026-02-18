#import <CoreLocation/CoreLocation.h>

#ifdef __cplusplus
class LocationManagerImpl;
#endif

@interface LocationManagerDelegateBridge : NSObject<CLLocationManagerDelegate>
@property (nonatomic, copy) void (^authStatusAuthAlwaysCB)(void);
@property (nonatomic, copy) void (^authStatusErrorCB)(void);
@property (nonatomic, copy) void (^locationChangedCB)(CLLocation*);
@end
